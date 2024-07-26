////////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: AGPL-3.0-only
// (C) Copyright 2022-2024 - formatBCE
// (C) Copyright 2024 - whitty

#include "format_presense_base.h"

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

namespace Format_Presence_Base {

std::string Format_Presence_Base::capitalizeString(const std::string& s) {
    std::string ret;
    ret.reserve(s.size());

    std::transform(s.begin(), s.end(), std::back_inserter(ret),
                   [](unsigned char c){ return std::toupper(c); });
    return ret;
}

void Format_Presence_Base::setup() {
    PollingComponent::setup();

    subscribe("format_ble_tracker/alive/+", &Format_Presence_Base::on_alive_message);
}

void Format_Presence_Base::report(const std::string& device, int rssi) {
    ESP_LOGD(TAG, "Sending for '%s': %ddBm", device.c_str(), rssi);

    const auto time = this->rtc_->timestamp_now();
    publish_json("format_ble_tracker/" + device + "/" + this->name_, [=](JsonObject root) {
        root["rssi"] = rssi;
        root["timestamp"] = time;
    }, 1, true);
}

void Format_Presence_Base::on_alive_message(const std::string &topic, const std::string &payload) {
    std::string uid = capitalizeString(topic.substr(topic.find_last_of("/") + 1));
    if (payload == "True") {
        if (uid.rfind(":") != std::string::npos) {
            if (std::find(this->macs_.begin(), this->macs_.end(), uid) == this->macs_.end()) {
                ESP_LOGD(TAG, "Adding MAC  %s", uid.c_str());
                this->macs_.push_back(uid);
            } else {
                ESP_LOGD(TAG, "Skipping duplicated MAC  %s", uid.c_str());
            }
        } else if (uid.rfind("-") != std::string::npos) {
            if (std::find(this->uuids_.begin(), this->uuids_.end(), uid) == this->uuids_.end()) {
                ESP_LOGD(TAG, "Adding UUID %s", uid.c_str());
                this->uuids_.push_back(uid);
            } else {
                ESP_LOGD(TAG, "Skipping duplicated UUID  %s", + uid.c_str());
            }
        }
        return;
    } else {
        ESP_LOGD(TAG, "Removing %s", uid.c_str());
        this->macs_.erase(std::remove(this->macs_.begin(), this->macs_.end(), uid), this->macs_.end());
        this->uuids_.erase(std::remove(this->uuids_.begin(), this->uuids_.end(), uid), this->uuids_.end());
    }
}

}
