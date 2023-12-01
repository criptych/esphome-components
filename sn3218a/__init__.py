import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["output"]

sn3218a_ns = cg.esphome_ns.namespace("sn3218a")
SN3218AComponent = sn3218a_ns.class_("SN3218AComponent", cg.Component, i2c.I2CDevice)

CONF_MAX_BRIGHTNESS = 'max_brightness'

CONFIG_SCHEMA = (
    cv.Schema({
        cv.GenerateID(): cv.declare_id(SN3218AComponent),
        cv.Optional(CONF_MAX_BRIGHTNESS, default=1.0): cv.float_,
        # TODO
    })
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(None))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_max_brightness(config[CONF_MAX_BRIGHTNESS]))
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
