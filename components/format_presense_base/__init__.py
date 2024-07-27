# SPDX-License-Identifier: AGPL-3.0-only
# (C) Copyright 2024 - Greg Whiteley

import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import time

from esphome.const import (
    CONF_ID,
    CONF_AREA,
    CONF_TIME_ID,
)

CODEOWNERS = ["@formatBCE", "@whitty"]
DEPENDENCIES = ["mqtt"]

Format_Presence_Base_ns = cg.esphome_ns.namespace("Format_Presence_Base")
Format_Presence_Base = Format_Presence_Base_ns.class_(
    "Format_Presence_Base",
    cg.PollingComponent,
    cg.esphome_ns.namespace("mqtt").class_("CustomMQTTDevice"))

CONFIG_SCHEMA_BASE = cv.Schema({
    cv.Required(CONF_AREA): cv.string,
    cv.Required(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code_base(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_area(config[CONF_AREA]))

    if CONF_TIME_ID in config:
        time_ = await cg.get_variable(config[CONF_TIME_ID])
        cg.add(var.set_time(time_))

    return var;
