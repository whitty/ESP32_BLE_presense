# SPDX-License-Identifier: AGPL-3.0-only
# (C) Copyright 2024 - Greg Whiteley

import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import time, esp32_ble_tracker

from esphome.const import (
    CONF_ID,
    CONF_AREA,
    CONF_TIME_ID,
)

CODEOWNERS = ["@formatBCE", "@whitty"]
DEPENDENCIES = ["esp32_ble_tracker"]

Format_BLE_Presense_ns = cg.esphome_ns.namespace("Format_BLE_Presense")
Format_BLE_Presense = Format_BLE_Presense_ns.class_(
    "Format_BLE_Presense",
    cg.Component,
    esp32_ble_tracker.ESPBTDeviceListener,
    cg.esphome_ns.namespace("mqtt").class_("CustomMQTTDevice"))

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Format_BLE_Presense),
    cv.Required(CONF_AREA): cv.string,
    cv.Required(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
}).extend(esp32_ble_tracker.ESP_BLE_DEVICE_SCHEMA).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await esp32_ble_tracker.register_ble_device(var, config)

    cg.add(var.set_area(config[CONF_AREA]))

    if CONF_TIME_ID in config:
        time_ = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.set_time(time_))
