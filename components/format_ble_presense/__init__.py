# SPDX-License-Identifier: AGPL-3.0-only
# (C) Copyright 2024 - Greg Whiteley

import esphome.codegen as cg
from esphome.components import esp32_ble_tracker
from ..format_presense_base import to_code_base, cv, CONFIG_SCHEMA_BASE, Format_Presence_Base

AUTO_LOAD = ["format_presense_base"]
CODEOWNERS = ["@formatBCE", "@whitty"]
DEPENDENCIES = ["esp32_ble_tracker"]

## TODO move this
Format_BLE_Presense_ns = cg.esphome_ns.namespace("Format_BLE_Presense")
Format_BLE_Presense = Format_BLE_Presense_ns.class_(
    "Format_BLE_Presense",
    Format_Presence_Base,
    esp32_ble_tracker.ESPBTDeviceListener)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Format_BLE_Presense),
}).extend(CONFIG_SCHEMA_BASE).extend(esp32_ble_tracker.ESP_BLE_DEVICE_SCHEMA)

async def to_code(config):
    var = await to_code_base(config)
    await esp32_ble_tracker.register_ble_device(var, config)
