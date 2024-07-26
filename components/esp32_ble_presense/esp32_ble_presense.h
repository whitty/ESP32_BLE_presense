////////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: AGPL-3.0-only
// (C) Copyright 2022-2024 - formatBCE

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/format_presense_base/format_presense_base.h"

namespace ESP32_BLE_Presense {

class ESP32_BLE_Presense : public Format_Presence_Base::Format_Presence_Base {
public:

    ESP32_BLE_Presense();
    ~ESP32_BLE_Presense() = default;

    void setup() override;
    void update() override;

    ESP32_BLE_Presense(const ESP32_BLE_Presense&) = delete;
    ESP32_BLE_Presense& operator=(const ESP32_BLE_Presense&) = delete;

private:

    friend class BleAdvertisedDeviceCallbacks;
    void reportDevice(const std::string& mac_address,
                      int rssi,
                      const std::string& strManufacturerData);
};

};
