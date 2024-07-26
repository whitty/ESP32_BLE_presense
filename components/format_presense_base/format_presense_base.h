////////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: AGPL-3.0-only
// (C) Copyright 2022-2024 - formatBCE
// (C) Copyright 2024 - whitty

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/mqtt/custom_mqtt_device.h"
#include "esphome/components/time/real_time_clock.h"

namespace Format_Presence_Base {

static constexpr const char TAG[] = "format_presense";

class Format_Presence_Base : public esphome::PollingComponent,
                             public esphome::mqtt::CustomMQTTDevice {
public:

    ~Format_Presence_Base() = default;

    void setup() override;
    void update() override {}

    void set_area(std::string area) {
        this->name_ = area;
    }

    void set_time(esphome::time::RealTimeClock* rtc) {
        this->rtc_ = rtc;
    }

    Format_Presence_Base(const Format_Presence_Base&) = delete;
    Format_Presence_Base& operator=(const Format_Presence_Base&) = delete;

protected:

    Format_Presence_Base() = default;
    Format_Presence_Base(uint32_t update_interval)
    :   PollingComponent(update_interval)
    {}

    void on_alive_message(const std::string &topic, const std::string &payload);

    void report(const std::string& device, int rssi);

    float get_setup_priority() const override { return esphome::setup_priority::AFTER_BLUETOOTH; }

    static std::string capitalizeString(const std::string& s);

private:

    std::string name_;

protected:
    std::vector<std::string> macs_;
    std::vector<std::string> uuids_;

private:
    esphome::time::RealTimeClock* rtc_ = 0;
};

};
