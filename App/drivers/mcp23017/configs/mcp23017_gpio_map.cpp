#include "mcp23017_gpio_map.hpp"

#include "enum_map.hpp"
#include "track_config.h"
#include "utils.h"

#define INDEX_TO_MASK(index) ((Mcp23017GpioPinMask)1 << index)

namespace Mcp23017GpioMap {

static constexpr const PinConfigMap pin_config_map{
    EnumEntry{Mcp23017GpioId::BUTTON_IFX_A_TOGGLE,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B100,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 2,
              }},
    EnumEntry{Mcp23017GpioId::LED_IFX_A,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = MCP23017_ADDRESS_0B100,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 3,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_ENCODER_A_PUSH,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B100,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 4,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_TRACK_1_EDIT,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_A,
                  .pin_index = 0,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_TRACK_1_PLAY_RECORD,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_A,
                  .pin_index = 1,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_TRACK_1_STOP,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_A,
                  .pin_index = 2,
              }},
    EnumEntry{Mcp23017GpioId::LED_TRACK_1_RED,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_A,
                  .pin_index = 3,
              }},
    EnumEntry{Mcp23017GpioId::LED_TRACK_1_GREEN,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_A,
                  .pin_index = 4,
              }},
    EnumEntry{Mcp23017GpioId::LED_TRACK_1_BLUE,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_A,
                  .pin_index = 5,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_LEFT,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 0,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_RIGHT,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 1,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_ENTER,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 2,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_EXIT,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 3,
              }},
    EnumEntry{Mcp23017GpioId::LED_TFX_A,
              PinConfig{
                  .gpio_type = GpioType::OUTPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 4,
              }},
    EnumEntry{Mcp23017GpioId::BUTTON_TFX_A_TOGGLE,
              PinConfig{
                  .gpio_type = GpioType::INPUT,
                  .address = MCP23017_ADDRESS_0B101,
                  .port = MCP23017_GPIO_PORT_B,
                  .pin_index = 5,
              }},
};

// TODO:
// TRACK_COUNT를 얻기 위해 track_state.h를 인클루드하는게 맞을까?
static Mcp23017GpioId track_led_gpio_table[TRACK_COUNT][(
    size_t)TrackLedColor::COUNT] = {
    {Mcp23017GpioId::NONE, Mcp23017GpioId::LED_TRACK_1_RED,
     Mcp23017GpioId::LED_TRACK_1_GREEN, Mcp23017GpioId::LED_TRACK_1_BLUE},
    // {0, Mcp23017GpioId::LED_TRACK_2_RED, Mcp23017GpioId::LED_TRACK_2_GREEN,
    //  Mcp23017GpioId::LED_TRACK_2_BLUE},
    // {0, Mcp23017GpioId::LED_TRACK_3_RED, Mcp23017GpioId::LED_TRACK_3_GREEN,
    //  Mcp23017GpioId::LED_TRACK_3_BLUE},
    // {0, Mcp23017GpioId::LED_TRACK_4_RED, Mcp23017GpioId::LED_TRACK_4_GREEN,
    //  Mcp23017GpioId::LED_TRACK_4_BLUE},
    // {0, Mcp23017GpioId::LED_TRACK_5_RED, Mcp23017GpioId::LED_TRACK_5_GREEN,
    //  Mcp23017GpioId::LED_TRACK_5_BLUE},
};

const PinConfig& GetTrackLedEntry(uint8_t track_index, TrackLedColor color) {
  return pin_config_map.Get(GetTrackLedGpioId(track_index, color));
}

Mcp23017GpioId GetTrackLedGpioId(uint8_t track_index, TrackLedColor color) {
  if (track_index >= TRACK_COUNT || color <= TrackLedColor::NONE ||
      color >= TrackLedColor::COUNT) {
    return Mcp23017GpioId::NONE;
  }
  return track_led_gpio_table[track_index][static_cast<size_t>(color)];
}

Mcp23017GpioId FindGpioIdFromPinConfig(Mcp23017Address address,
                                       Mcp23017Port port,
                                       std::uint8_t pin_index) {
  for (size_t i = 0; i < static_cast<size_t>(Mcp23017GpioId::COUNT); i++) {
    const Mcp23017GpioId gpio_id = static_cast<Mcp23017GpioId>(i);
    const PinConfig& value = pin_config_map[gpio_id];

    if (value.address == address && value.port == port &&
        value.pin_index == pin_index) {
      return gpio_id;
    }
  }
  return Mcp23017GpioId::NONE;
}

Mcp23017PinMask GetInputPinMaskFromAddressAndPort(Mcp23017Address address,
                                                  Mcp23017Port port) {
  Mcp23017PinMask input_pin_mask = 0;

  for (size_t i = 0; i < static_cast<size_t>(Mcp23017GpioId::COUNT); i++) {
    const Mcp23017GpioId gpio_id = static_cast<Mcp23017GpioId>(i);
    if (pin_config_map[gpio_id].address == address &&
        pin_config_map[gpio_id].port == port &&
        pin_config_map[gpio_id].gpio_type == GpioType::INPUT) {
      input_pin_mask |= 1 << pin_config_map[gpio_id].pin_index;
    }
  }
  return input_pin_mask;
}

const PinConfigMap& GetEnumMap() { return pin_config_map; }

}  // namespace Mcp23017GpioMap
