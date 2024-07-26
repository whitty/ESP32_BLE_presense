////////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: AGPL-3.0-only
// (C) Copyright 2024 - whitty

#include "format_ble_presense.h"

#include "esphome/core/log.h"

// Undo the default loose definitions in our file only
#pragma GCC diagnostic error "-Wdeprecated-declarations"
#pragma GCC diagnostic error "-Wsign-compare"
#pragma GCC diagnostic error "-Wunused-but-set-variable"
#pragma GCC diagnostic error "-Wunused-function"
#pragma GCC diagnostic error "-Wunused-parameter"
#pragma GCC diagnostic error "-Wunused-variable"
#pragma GCC diagnostic error "-Wformat"

using namespace esphome;

namespace Format_BLE_Presense {

bool Format_BLE_Presense::parse_device(const esphome::esp32_ble_tracker::ESPBTDevice &device) {
    const auto rssi = device.get_rssi();
    const auto addr_str = device.address_str();

    if (std::find(this->macs_.begin(), this->macs_.end(), addr_str) != this->macs_.end()) {
        report(addr_str, rssi);
        return true;
    }

    auto ibeacon = device.get_ibeacon();
    if (ibeacon) {
        const auto ibeacon_uuid = ibeacon->get_uuid();
        const auto uuid_str = ibeacon_uuid.to_string();

        if (std::find(this->uuids_.begin(), this->uuids_.end(), uuid_str) != this->uuids_.end()) {
            report(uuid_str, rssi);
            return true;
        }

#ifdef ESPHOME_LOG_HAS_VERBOSE
        const auto ibeacon_minor = ibeacon->get_minor();
        const auto ibeacon_major = ibeacon->get_major();
        ESP_LOGV(TAG, "ibeacon '%s': %ddBm major=%u minor=%u", uuid_str.c_str(), rssi, ibeacon_major, ibeacon_minor);
#endif
    }

#ifdef ESPHOME_LOG_HAS_VERBOSE
    ESP_LOGV(TAG, "addr '%s': %ddBm", addr_str.c_str(), rssi);
    for (const auto uuid : device.get_service_uuids()) {
        ESP_LOGV(TAG, "uuid '%s': %ddBm", uuid.to_string().c_str(), rssi);
    }
#endif

    return false;
}

} // Format_BLE_Presense
