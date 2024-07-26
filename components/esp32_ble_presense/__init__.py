# SPDX-License-Identifier: AGPL-3.0-only
# (C) Copyright 2024 - Greg Whiteley

import esphome.codegen as cg
from ..format_presense_base import to_code_base, cv, CONFIG_SCHEMA_BASE, Format_Presence_Base

AUTO_LOAD = ["format_presense_base"]
CODEOWNERS = ["@formatBCE"]

## TODO move this
ESP32_BLE_Presense_ns = cg.esphome_ns.namespace("ESP32_BLE_Presense")
ESP32_BLE_Presense = ESP32_BLE_Presense_ns.class_(
    "ESP32_BLE_Presense",
    Format_Presence_Base)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ESP32_BLE_Presense),
}).extend(CONFIG_SCHEMA_BASE)

async def to_code(config):
    cg.add_library(
        name="NimBLE-Arduino",
        version="1.4.2",
    )

    await to_code_base(config)
