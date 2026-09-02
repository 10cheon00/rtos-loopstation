#include "mcp23017_gpio_map.hpp"

#include "enum_map.hpp"
#include "track_config.h"
#include "utils.h"

#define INDEX_TO_MASK(index) ((Mcp23017GpioPinMask)1 << index)

namespace Mcp23017 {

static constexpr const PinConfigMap pin_config_map{
    EnumEntry{GpioId::BUTTON_IFX_A_TOGGLE,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b100,
                  .port = Port::B,
                  .pin_index = 2,
              }},
    EnumEntry{GpioId::LED_IFX_A,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = Address::b100,
                  .port = Port::B,
                  .pin_index = 3,
              }},
    EnumEntry{GpioId::BUTTON_ENCODER_A_PUSH,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b100,
                  .port = Port::B,
                  .pin_index = 4,
              }},
    EnumEntry{GpioId::BUTTON_TRACK_1_EDIT,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b101,
                  .port = Port::A,
                  .pin_index = 0,
              }},
    EnumEntry{GpioId::BUTTON_TRACK_1_PLAY_RECORD,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b101,
                  .port = Port::A,
                  .pin_index = 1,
              }},
    EnumEntry{GpioId::BUTTON_TRACK_1_STOP,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b101,
                  .port = Port::A,
                  .pin_index = 2,
              }},
    EnumEntry{GpioId::LED_TRACK_1_RED,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = Address::b101,
                  .port = Port::A,
                  .pin_index = 3,
              }},
    EnumEntry{GpioId::LED_TRACK_1_GREEN,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = Address::b101,
                  .port = Port::A,
                  .pin_index = 4,
              }},
    EnumEntry{GpioId::LED_TRACK_1_BLUE,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = Address::b101,
                  .port = Port::A,
                  .pin_index = 5,
              }},
    EnumEntry{GpioId::BUTTON_LEFT,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b101,
                  .port = Port::B,
                  .pin_index = 0,
              }},
    EnumEntry{GpioId::BUTTON_RIGHT,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b101,
                  .port = Port::B,
                  .pin_index = 1,
              }},
    EnumEntry{GpioId::BUTTON_ENTER,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b101,
                  .port = Port::B,
                  .pin_index = 2,
              }},
    EnumEntry{GpioId::BUTTON_EXIT,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b101,
                  .port = Port::B,
                  .pin_index = 3,
              }},
    EnumEntry{GpioId::LED_TFX_A,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = Address::b101,
                  .port = Port::B,
                  .pin_index = 4,
              }},
    EnumEntry{GpioId::BUTTON_TFX_A_TOGGLE,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = Address::b101,
                  .port = Port::B,
                  .pin_index = 5,
              }},
};

// TODO:
// TRACK_COUNT를 얻기 위해 track_state.h를 인클루드하는게 맞을까?
static GpioId track_led_gpio_table[TRACK_COUNT][(size_t)TrackLedColor::COUNT] =
    {
        {GpioId::NONE, GpioId::LED_TRACK_1_RED, GpioId::LED_TRACK_1_GREEN,
         GpioId::LED_TRACK_1_BLUE},
        // {0, GpioId::LED_TRACK_2_RED,
        // GpioId::LED_TRACK_2_GREEN,
        //  GpioId::LED_TRACK_2_BLUE},
        // {0, GpioId::LED_TRACK_3_RED,
        // GpioId::LED_TRACK_3_GREEN,
        //  GpioId::LED_TRACK_3_BLUE},
        // {0, GpioId::LED_TRACK_4_RED,
        // GpioId::LED_TRACK_4_GREEN,
        //  GpioId::LED_TRACK_4_BLUE},
        // {0, GpioId::LED_TRACK_5_RED,
        // GpioId::LED_TRACK_5_GREEN,
        //  GpioId::LED_TRACK_5_BLUE},
};

const PinConfig& GetTrackLedEntry(uint8_t track_index, TrackLedColor color) {
  return pin_config_map.Get(GetTrackLedGpioId(track_index, color));
}

GpioId GetTrackLedGpioId(uint8_t track_index, TrackLedColor color) {
  if (track_index >= TRACK_COUNT || color <= TrackLedColor::NONE ||
      color >= TrackLedColor::COUNT) {
    return GpioId::NONE;
  }
  return track_led_gpio_table[track_index][static_cast<size_t>(color)];
}

GpioId FindGpioIdFromPinConfig(Address address, Port port, PinIndex pin_index) {
  for (size_t i = 0; i < static_cast<size_t>(GpioId::COUNT); i++) {
    const GpioId gpio_id = static_cast<GpioId>(i);
    const PinConfig& value = pin_config_map[gpio_id];

    if (value.address == address && value.port == port &&
        value.pin_index == pin_index) {
      return gpio_id;
    }
  }
  return GpioId::NONE;
}

PinMask GetInputPinMaskFromAddressAndPort(Address address, Port port) {
  PinMask input_pin_mask = 0;

  for (size_t i = 0; i < static_cast<size_t>(GpioId::COUNT); i++) {
    const GpioId gpio_id = static_cast<GpioId>(i);
    if (pin_config_map[gpio_id].address == address &&
        pin_config_map[gpio_id].port == port &&
        pin_config_map[gpio_id].gpio_type == GpioType::INPUT) {
      input_pin_mask |= 1 << pin_config_map[gpio_id].pin_index;
    }
  }
  return input_pin_mask;
}

const PinConfigMap& GetPinConfigMap() { return pin_config_map; }

}  // namespace Mcp23017
