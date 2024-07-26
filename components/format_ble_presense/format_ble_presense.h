////////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: AGPL-3.0-only
// (C) Copyright 2022-2024 - formatBCE
// (C) Copyright 2024 - whitty

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/format_presense_base/format_presense_base.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

namespace Format_BLE_Presense {

static constexpr const char TAG[] = "format_ble_presense";

class Format_BLE_Presense : public Format_Presence_Base::Format_Presence_Base,
                            public esphome::esp32_ble_tracker::ESPBTDeviceListener {
public:

    Format_BLE_Presense() = default;
    ~Format_BLE_Presense() = default;

    Format_BLE_Presense(const Format_BLE_Presense&) = delete;
    Format_BLE_Presense& operator=(const Format_BLE_Presense&) = delete;

private:

    virtual bool parse_device(const esphome::esp32_ble_tracker::ESPBTDevice &device) override;
};

};
