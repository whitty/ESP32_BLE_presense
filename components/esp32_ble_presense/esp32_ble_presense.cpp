////////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: AGPL-3.0-only
// (C) Copyright 2022-2024 - formatBCE

#include "esp32_ble_presense.h"

#include "esphome/core/log.h"

#include <NimBLEDevice.h>

#define bleScanInterval 0x80 // Used to determine antenna sharing between Bluetooth and WiFi. Do not modify unless you are confident you know what you're doing
#define bleScanWindow 0x40 // Used to determine antenna sharing between Bluetooth and WiFi. Do not modify unless you are confident you know what you're doing

// Undo the default loose definitions in our file only
#pragma GCC diagnostic error "-Wdeprecated-declarations"
#pragma GCC diagnostic error "-Wsign-compare"
#pragma GCC diagnostic error "-Wunused-but-set-variable"
#pragma GCC diagnostic error "-Wunused-function"
#pragma GCC diagnostic error "-Wunused-parameter"
#pragma GCC diagnostic error "-Wunused-variable"
#pragma GCC diagnostic error "-Wformat"

using namespace esphome;

namespace ESP32_BLE_Presense {

NimBLEScan* pBLEScan;

class BleAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {

    ESP32_BLE_Presense& parent_;

public:
    BleAdvertisedDeviceCallbacks(ESP32_BLE_Presense& parent) : parent_(parent) {}

    void onResult(NimBLEAdvertisedDevice* device) {
        if (!device)
            return;

        parent_.reportDevice(device->getAddress().toString(),
                             device->getRSSI(),
                             device->getManufacturerData());
    }
};

ESP32_BLE_Presense::ESP32_BLE_Presense()
:  Format_Presence_Base(5000) {
}

void ESP32_BLE_Presense::update() {
    if (!pBLEScan->isScanning()) {
        ESP_LOGD("format_ble", "Start scanning...");
        pBLEScan->start(0, nullptr, false);
    }
    ESP_LOGD("format_ble", "BLE scan heartbeat");
}

void ESP32_BLE_Presense::setup() {
    Format_Presence_Base::setup();

    NimBLEDevice::init("");
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    pBLEScan = NimBLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new BleAdvertisedDeviceCallbacks(*this), true);
    pBLEScan->setInterval(bleScanInterval);
    pBLEScan->setWindow(bleScanWindow);
    pBLEScan->setActiveScan(false);
    pBLEScan->setMaxResults(0);
}

void ESP32_BLE_Presense::reportDevice(const std::string& macAddress,
                                    int rssi,
                                    const std::string& manufacturerData) {

    std::string mac_address = capitalizeString(macAddress);
    if (std::find(this->macs_.begin(), this->macs_.end(), mac_address) != this->macs_.end()) {
        report(mac_address, rssi);
        return;
    }

    static const size_t UUID_INDEX = 4;
    static const size_t UUID_LEN = 16;
    if (manufacturerData.length() >= UUID_INDEX + UUID_LEN) {
        std::string uuid_str = capitalizeString(NimBLEUUID(reinterpret_cast<const uint8_t*>(&manufacturerData[UUID_INDEX]),
                                                           UUID_LEN, true).toString());
        if (std::find(this->uuids_.begin(), this->uuids_.end(), uuid_str) != this->uuids_.end()) {
            report(uuid_str, rssi);
        }
    }
}

}
