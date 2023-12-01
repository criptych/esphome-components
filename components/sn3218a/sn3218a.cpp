#include "sn3218a.h"

namespace esphome {
namespace sn3218a {

static const char * const TAG = "sn3218a";

void SN3218AComponent::setup()
{
    constexpr uint8_t data[] = {
        // master enable
        0x01,
        // initial levels
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // select LEDs
        0x3f, 0x3f, 0x3f,
        // update
        0xff,
    };

    this->write_register(SN3218A_CMD_ENABLE, data, sizeof data);
}

void SN3218AComponent::dump_config()
{
    ESP_LOGCONFIG(TAG, "SN3218A:");
    LOG_I2C_DEVICE(this);
}

void SN3218AComponent::loop()
{
    if (update_)
    {
        // TODO optimize how many values to update?
        this->write_register(SN3218A_CMD_LEVEL, values_, sizeof values_);

        uint8_t select[] = {
            (select_ >>  0) & 0x3f,
            (select_ >>  6) & 0x3f,
            (select_ >> 12) & 0x3f,
        };
        this->write_register(SN3218A_CMD_SELECT, select, sizeof select);

        constexpr uint8_t dummy[] = { 0xff };
        this->write_register(SN3218A_CMD_UPDATE, dummy, sizeof dummy);
        update_ = 0;
    }
}

void SN3218AComponent::set_led_brightness_(uint8_t index, float state)
{
    if (index < SN3218A_LED_COUNT)
    {
        uint8_t value = static_cast<uint8_t>(255 * state * max_brightness_);
        values_[index] = value;
        update_ |= (1u << index);
    }
}

void SN3218AComponent::set_led_select_(uint8_t index, bool selected)
{
    if (index < SN3218A_LED_COUNT)
    {
        uint32_t mask = 1u << index;
        select_ = (select_ & ~mask) | (selected * mask);
        update_ |= mask;
    }
}

} // namespace sn3218a
} // namespace esphome
