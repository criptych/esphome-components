#pragma once

#include "esphome.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sn3218a {

constexpr uint8_t SN3218A_LED_COUNT = 18;
constexpr uint8_t SN3218A_CMD_ENABLE = 0x00; // master enable
constexpr uint8_t SN3218A_CMD_LEVEL  = 0x01; //  1..18, 8-bit brightness level
constexpr uint8_t SN3218A_CMD_SELECT = 0x13; // 19..21, 6 bitflags each
constexpr uint8_t SN3218A_CMD_UPDATE = 0x16; // update PWM
constexpr uint8_t SN3218A_CMD_RESET  = 0x17; // reset device

class SN3218AComponent : public Component, public i2c::I2CDevice
{
public:
    SN3218AComponent(): max_brightness_(1.f), update_(0), select_(0x3ffff) {}

    float get_setup_priority() const override
    {
        return setup_priority::HARDWARE;
    }

    void setup() override;
    void dump_config() override;
    void loop() override;

    void set_max_brightness(float brightness)
    {
        max_brightness_ = brightness;
        update_ |= 0x8000000;
    }

protected:
    friend class SN3218AOutput;

    void set_led_brightness_(uint8_t index, float value);
    float get_led_brightness_(uint8_t index) const;
    void set_led_select_(uint8_t index, bool selected);
    bool get_led_select_(uint8_t index) const;

    float max_brightness_;
    uint32_t update_;
    uint32_t select_;
    uint8_t values_[SN3218A_LED_COUNT];
};

class SN3218AOutput : public output::FloatOutput
{
public:
    SN3218AOutput(SN3218AComponent *parent, uint8_t index)
        : parent_(parent), index_(index)
    {
    }

    void set_selected(bool selected = true)
    {
        parent_->set_led_select_(index_, selected);
    }

protected:
    void write_state(float state) override
    {
        parent_->set_led_brightness_(index_, state);
    }

    SN3218AComponent *parent_;
    uint8_t index_;
};

} // namespace sn3218a
} // namespace esphome
