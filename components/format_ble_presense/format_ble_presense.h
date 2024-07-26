////////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: AGPL-3.0-only
// (C) Copyright 2022-2024 - formatBCE

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/mqtt/custom_mqtt_device.h"
#include "esphome/components/time/real_time_clock.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

namespace Format_BLE_Presense {

static constexpr const char TAG[] = "format_ble_presense";

class Format_BLE_Presense : public esphome::Component,
                            public esphome::esp32_ble_tracker::ESPBTDeviceListener,
                            public esphome::mqtt::CustomMQTTDevice {
    std::string name_;

    std::vector<std::string> macs_;
    std::vector<std::string> uuids_;

    esphome::time::RealTimeClock* rtc_ = 0;

public:

    Format_BLE_Presense() = default;
    ~Format_BLE_Presense() = default;

    void setup() override;

    void set_area(std::string area) {
        this->name_ = area;
    }

    void set_time(esphome::time::RealTimeClock* rtc) {
        this->rtc_ = rtc;
    }

    Format_BLE_Presense(const Format_BLE_Presense&) = delete;
    Format_BLE_Presense& operator=(const Format_BLE_Presense&) = delete;

private:

    virtual bool parse_device(const esphome::esp32_ble_tracker::ESPBTDevice &device) override;

    void on_alive_message(const std::string &topic, const std::string &payload);

    void report(const std::string& device, int rssi);

    float get_setup_priority() const override { return esphome::setup_priority::AFTER_BLUETOOTH; }
};

};
