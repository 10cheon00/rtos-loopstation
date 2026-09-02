#include "mcp23017_gpio_map.hpp"

#include "config_table.h"
#include "config_validator.h"
#include "track_state.h"
#include "utils.h"

#define INDEX_TO_MASK(index) ((Mcp23017GpioPinMask)1 << index)

static ParameterPinMapEntry parameter_pin_map[] = {
    {
        .gpio_id = Mcp23017GpioId::BUTTON_IFX_A_TOGGLE,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B100,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 2,
    },
    {
        .gpio_id = Mcp23017GpioId::LED_IFX_A,
        .gpio_type = Mcp23017GpioType::OUTPUT,
        .address = MCP23017_ADDRESS_0B100,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 3,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_ENCODER_A_PUSH,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B100,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 4,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_TRACK_1_EDIT,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .pin_index = 0,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_TRACK_1_PLAY_RECORD,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .pin_index = 1,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_TRACK_1_STOP,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .pin_index = 2,
    },
    {
        .gpio_id = Mcp23017GpioId::LED_TRACK_1_RED,
        .gpio_type = Mcp23017GpioType::OUTPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .pin_index = 3,
    },
    {
        .gpio_id = Mcp23017GpioId::LED_TRACK_1_GREEN,
        .gpio_type = Mcp23017GpioType::OUTPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .pin_index = 4,
    },
    {
        .gpio_id = Mcp23017GpioId::LED_TRACK_1_BLUE,
        .gpio_type = Mcp23017GpioType::OUTPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .pin_index = 5,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_LEFT,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 0,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_RIGHT,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 1,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_ENTER,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 2,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_EXIT,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 3,
    },
    {
        .gpio_id = Mcp23017GpioId::LED_TFX_A,
        .gpio_type = Mcp23017GpioType::OUTPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 4,
    },
    {
        .gpio_id = Mcp23017GpioId::BUTTON_TFX_A_TOGGLE,
        .gpio_type = Mcp23017GpioType::INPUT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .pin_index = 5,
    },
};

static const size_t parameter_pin_map_count = ARRAY_COUNT(parameter_pin_map);

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

ParameterPinMapEntry* Mcp23017GpioMap_GetEntry(Mcp23017GpioId gpio_id) {
  for (size_t i = 0; i < parameter_pin_map_count; i++) {
    if (parameter_pin_map[i].gpio_id == gpio_id) {
      return &parameter_pin_map[i];
    }
  }
  return NULL;
}

ParameterPinMapEntry* Mcp23017GpioMap_GetTrackLedEntry(uint8_t track_index,
                                                       TrackLedColor color) {
  Mcp23017GpioId gpio_id;
  gpio_id = track_led_gpio_table[track_index][(size_t)color];

  for (size_t i = 0; i < parameter_pin_map_count; i++) {
    if (parameter_pin_map[i].gpio_id == gpio_id) {
      return &parameter_pin_map[i];
    }
  }
  return NULL;
}

Mcp23017GpioId Mcp23017GpioMap_GetMcp23017GpioId(Mcp23017Address address,
                                                 Mcp23017Port port,
                                                 uint8_t pin_index) {
  for (size_t i = 0; i < parameter_pin_map_count; i++) {
    if (parameter_pin_map[i].address == address &&
        parameter_pin_map[i].port == port &&
        parameter_pin_map[i].pin_index == pin_index) {
      return parameter_pin_map[i].gpio_id;
    }
  }
  return Mcp23017GpioId::NONE;
}

Mcp23017PinMask Mcp23017GpioMap_GetInputPinMask(Mcp23017Address address,
                                                Mcp23017Port port) {
  Mcp23017PinMask input_pin_mask = 0;

  for (size_t i = 0; i < parameter_pin_map_count; i++) {
    if (parameter_pin_map[i].address == address &&
        parameter_pin_map[i].port == port &&
        parameter_pin_map[i].gpio_type == Mcp23017GpioType::INPUT) {
      input_pin_mask |= 1 << parameter_pin_map[i].pin_index;
    }
  }
  return input_pin_mask;
}

Mcp23017Port Mcp23017GpioMap_GetPortFromAddressAndGpioId(
    Mcp23017Address address, Mcp23017GpioId gpio_id) {
  for (size_t i = 0; i < parameter_pin_map_count; i++) {
    if (parameter_pin_map[i].address == address &&
        parameter_pin_map[i].gpio_id == gpio_id) {
      return parameter_pin_map[i].port;
    }
  }
  // TODO:
  // 반환값이 항상 올바른 것이 아니므로, Status를 반환하도록 전부 고치기?
  return (Mcp23017Port)0xFF;
}

/**
 * Mcp23017GpioId는 ButtonId와 유사하지만 입력 + 출력용 Gpio들을 모두 모은
 * 것이다. 따라서 입력용 Gpio들을 ButtonId로 변환시켜주려면 테이블이 필요하다.
 * 이 테이블은 수동으로 컨버팅한다.
 */
static constexpr auto gpio_to_button_map = [] {
  std::array<ButtonId, (size_t)(Mcp23017GpioId::COUNT)> values{};
  values[(size_t)Mcp23017GpioId::NONE] = BUTTON_ID_NONE;
  values[(size_t)Mcp23017GpioId::LED_IFX_A] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TFX_A] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::BUTTON_LEFT] = BUTTON_ID_LEFT;
  values[(size_t)Mcp23017GpioId::BUTTON_RIGHT] = BUTTON_ID_RIGHT;
  values[(size_t)Mcp23017GpioId::BUTTON_ENTER] = BUTTON_ID_ENTER;
  values[(size_t)Mcp23017GpioId::BUTTON_EXIT] = BUTTON_ID_EXIT;
  values[(size_t)Mcp23017GpioId::BUTTON_ENCODER_A_PUSH] =
      BUTTON_ID_ENCODER_A_PUSH;
  values[(size_t)Mcp23017GpioId::BUTTON_ENCODER_B_PUSH] =
      BUTTON_ID_ENCODER_B_PUSH;
  values[(size_t)Mcp23017GpioId::BUTTON_ENCODER_C_PUSH] =
      BUTTON_ID_ENCODER_C_PUSH;
  values[(size_t)Mcp23017GpioId::BUTTON_ENCODER_D_PUSH] =
      BUTTON_ID_ENCODER_D_PUSH;
  values[(size_t)Mcp23017GpioId::BUTTON_IFX_A_TOGGLE] = BUTTON_ID_IFX_A_TOGGLE;
  values[(size_t)Mcp23017GpioId::BUTTON_TFX_A_TOGGLE] = BUTTON_ID_TFX_A_TOGGLE;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_1_EDIT] = BUTTON_ID_TRACK_1_EDIT;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_1_PLAY_RECORD] =
      BUTTON_ID_TRACK_1_PLAY_RECORD;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_1_STOP] = BUTTON_ID_TRACK_1_STOP;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_2_EDIT] = BUTTON_ID_TRACK_2_EDIT;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_2_PLAY_RECORD] =
      BUTTON_ID_TRACK_2_PLAY_RECORD;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_2_STOP] = BUTTON_ID_TRACK_2_STOP;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_3_EDIT] = BUTTON_ID_TRACK_3_EDIT;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_3_PLAY_RECORD] =
      BUTTON_ID_TRACK_3_PLAY_RECORD;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_3_STOP] = BUTTON_ID_TRACK_3_STOP;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_4_EDIT] = BUTTON_ID_TRACK_4_EDIT;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_4_PLAY_RECORD] =
      BUTTON_ID_TRACK_4_PLAY_RECORD;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_4_STOP] = BUTTON_ID_TRACK_4_STOP;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_5_EDIT] = BUTTON_ID_TRACK_5_EDIT;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_5_PLAY_RECORD] =
      BUTTON_ID_TRACK_5_PLAY_RECORD;
  values[(size_t)Mcp23017GpioId::BUTTON_TRACK_5_STOP] = BUTTON_ID_TRACK_5_STOP;
  values[(size_t)Mcp23017GpioId::LED_TRACK_1_RED] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_1_GREEN] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_1_BLUE] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_2_RED] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_2_GREEN] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_2_BLUE] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_3_RED] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_3_GREEN] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_3_BLUE] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_4_RED] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_4_GREEN] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_4_BLUE] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_5_RED] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_5_GREEN] = BUTTON_ID_NULL;
  values[(size_t)Mcp23017GpioId::LED_TRACK_5_BLUE] = BUTTON_ID_NULL;
  return values;
}();

ButtonId Mcp23017GpioMap_Get(Mcp23017GpioId gpio_id) {
  if (gpio_id <= Mcp23017GpioId::NONE || gpio_id >= Mcp23017GpioId::COUNT) {
    return BUTTON_ID_NONE;
  }
  return gpio_to_button_map[(size_t)gpio_id];
}
