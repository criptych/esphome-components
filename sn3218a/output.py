import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID
from . import SN3218AComponent, sn3218a_ns

DEPENDENCIES = ["sn3218a"]

SN3218AOutput = sn3218a_ns.class_("SN3218AOutput", output.FloatOutput)

CONF_SN3218A_ID = 'sn3218a_id'
CONF_INDEX = 'index'

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend({
    cv.Required(CONF_ID): cv.declare_id(SN3218AOutput),
    cv.GenerateID(CONF_SN3218A_ID): cv.use_id(SN3218AComponent),
    cv.Required(CONF_INDEX): cv.int_,
})

async def to_code(config):
    paren = await cg.get_variable(config[CONF_SN3218A_ID])
    var = cg.new_Pvariable(
        config[CONF_ID],
        paren,
        config[CONF_INDEX]
    )
    await output.register_output(var, config)
