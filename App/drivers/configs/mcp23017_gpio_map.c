#include "mcp23017_gpio_map.h"

#include "utils.h"
#include "track_state.h"
#include "config_table.h"

#define INDEX_TO_MASK(index) ((Mcp23017GpioPinMask)1 << index)

static ParameterPinMapEntry parameter_pin_map[] = {
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_IFX_A_TOGGLE,
        .address = MCP23017_ADDRESS_0B100,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(2),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_LED_IFX_A,
        .address = MCP23017_ADDRESS_0B100,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(3),
        .gpio_type = MCP23017_GPIO_TYPE_OUTPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_ENCODER_A_PUSH,
        .address = MCP23017_ADDRESS_0B100,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(4),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_TRACK_1_EDIT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .gpio_pin_mask = INDEX_TO_MASK(0),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_TRACK_1_PLAY_RECORD,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .gpio_pin_mask = INDEX_TO_MASK(1),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_TRACK_1_STOP,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .gpio_pin_mask = INDEX_TO_MASK(2),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_LED_TRACK_1_RED,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .gpio_pin_mask = INDEX_TO_MASK(3),
        .gpio_type = MCP23017_GPIO_TYPE_OUTPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_LED_TRACK_1_GREEN,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .gpio_pin_mask = INDEX_TO_MASK(4),
        .gpio_type = MCP23017_GPIO_TYPE_OUTPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_LED_TRACK_1_BLUE,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_A,
        .gpio_pin_mask = INDEX_TO_MASK(5),
        .gpio_type = MCP23017_GPIO_TYPE_OUTPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_LEFT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(0),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_RIGHT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(1),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_ENTER,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(2),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_EXIT,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(3),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_LED_TFX_A,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(4),
        .gpio_type = MCP23017_GPIO_TYPE_OUTPUT,
    },
    {
        .gpio_id = MCP23017_GPIO_ID_BUTTON_TFX_A_TOGGLE,
        .address = MCP23017_ADDRESS_0B101,
        .port = MCP23017_GPIO_PORT_B,
        .gpio_pin_mask = INDEX_TO_MASK(5),
        .gpio_type = MCP23017_GPIO_TYPE_INPUT,
    },
};

static const size_t parameter_pin_map_count = ARRAY_COUNT(parameter_pin_map);

// TODO:
// TRACK_COUNT를 얻기 위해 track_state.h를 인클루드하는게 맞을까?
static Mcp23017GpioId track_led_gpio_table[TRACK_COUNT][TRACK_LED_COLOR_COUNT] = {
    {0, MCP23017_GPIO_ID_LED_TRACK_1_RED, MCP23017_GPIO_ID_LED_TRACK_1_GREEN,
     MCP23017_GPIO_ID_LED_TRACK_1_BLUE},
    {0, MCP23017_GPIO_ID_LED_TRACK_2_RED, MCP23017_GPIO_ID_LED_TRACK_2_GREEN,
     MCP23017_GPIO_ID_LED_TRACK_2_BLUE},
    {0, MCP23017_GPIO_ID_LED_TRACK_3_RED, MCP23017_GPIO_ID_LED_TRACK_3_GREEN,
     MCP23017_GPIO_ID_LED_TRACK_3_BLUE},
    {0, MCP23017_GPIO_ID_LED_TRACK_4_RED, MCP23017_GPIO_ID_LED_TRACK_4_GREEN,
     MCP23017_GPIO_ID_LED_TRACK_4_BLUE},
    {0, MCP23017_GPIO_ID_LED_TRACK_5_RED, MCP23017_GPIO_ID_LED_TRACK_5_GREEN,
     MCP23017_GPIO_ID_LED_TRACK_5_BLUE},
};

ParameterPinMapEntry *Mcp23017GpioMap_GetEntry(Mcp23017GpioId gpio_id)
{
    for (size_t i = 0; i < parameter_pin_map_count; i++) {
        if (parameter_pin_map[i].gpio_id == gpio_id) {
            return &parameter_pin_map[i];
        }
    }
    return NULL;
}

ParameterPinMapEntry *Mcp23017GpioMap_GetTrackLedEntry(uint8_t track_index, TrackLedColor color)
{

    Mcp23017GpioId gpio_id;
    // TODO:
    // 하드코딩이므로 고쳐야함
    if (track_index < 0 || track_index >= TRACK_COUNT || color < TRACK_LED_COLOR_NONE ||
        color >= TRACK_LED_COLOR_COUNT) {
        return NULL;
    }
    gpio_id = track_led_gpio_table[track_index][color];

    for (size_t i = 0; i < parameter_pin_map_count; i++) {
        if (parameter_pin_map[i].gpio_id == gpio_id) {
            return &parameter_pin_map[i];
        }
    }
    return NULL;
}

Mcp23017GpioId Mcp23017GpioMap_GetMcp23017GpioId(Mcp23017Address address, Mcp23017GpioPort port,
                                                 Mcp23017GpioPinMask gpio_pin_mask)
{
    for (size_t i = 0; i < parameter_pin_map_count; i++) {
        if (parameter_pin_map[i].address == address && parameter_pin_map[i].port == port &&
            parameter_pin_map[i].gpio_pin_mask == gpio_pin_mask) {
            return parameter_pin_map[i].gpio_id;
        }
    }
    return 0;
}

Mcp23017GpioPinMask Mcp23017GpioMap_GetInputPinMask(Mcp23017Address address, Mcp23017GpioPort port)
{
    Mcp23017GpioPinMask input_pin_mask = 0;

    for (size_t i = 0; i < parameter_pin_map_count; i++) {
        if (parameter_pin_map[i].address == address && parameter_pin_map[i].port == port &&
            parameter_pin_map[i].gpio_type == MCP23017_GPIO_TYPE_INPUT) {
            input_pin_mask |= parameter_pin_map[i].gpio_pin_mask;
        }
    }
    return input_pin_mask;
}

/**
 * Mcp23017GpioId는 ButtonId와 유사하지만 입력 + 출력용 Gpio들을 모두 모은 것이다.
 * 따라서 입력용 Gpio들을 ButtonId로 변환시켜주려면 테이블이 필요하다.
 * 이 테이블은 수동으로 컨버팅한다.
 */
#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_NONE, BUTTON_ID_NONE),                                   \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_LED_IFX_A, BUTTON_ID_NONE),                          \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_LED_TFX_A, BUTTON_ID_NONE),                          \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_LEFT, BUTTON_ID_LEFT),                        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_RIGHT, BUTTON_ID_RIGHT),                      \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_ENTER, BUTTON_ID_ENTER),                      \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_EXIT, BUTTON_ID_EXIT),                        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_ENCODER_A_PUSH, BUTTON_ID_ENCODER_A_PUSH),    \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_ENCODER_B_PUSH, BUTTON_ID_ENCODER_B_PUSH),    \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_ENCODER_C_PUSH, BUTTON_ID_ENCODER_C_PUSH),    \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_ENCODER_D_PUSH, BUTTON_ID_ENCODER_D_PUSH),    \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_IFX_A_TOGGLE, BUTTON_ID_IFX_A_TOGGLE),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TFX_A_TOGGLE, BUTTON_ID_TFX_A_TOGGLE),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_1_EDIT, BUTTON_ID_TRACK_1_EDIT),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_1_PLAY_RECORD,                          \
                             BUTTON_ID_TRACK_1_PLAY_RECORD),                                       \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_1_STOP, BUTTON_ID_TRACK_1_STOP),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_2_EDIT, BUTTON_ID_TRACK_2_EDIT),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_2_PLAY_RECORD,                          \
                             BUTTON_ID_TRACK_2_PLAY_RECORD),                                       \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_2_STOP, BUTTON_ID_TRACK_2_STOP),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_3_EDIT, BUTTON_ID_TRACK_3_EDIT),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_3_PLAY_RECORD,                          \
                             BUTTON_ID_TRACK_3_PLAY_RECORD),                                       \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_3_STOP, BUTTON_ID_TRACK_3_STOP),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_4_EDIT, BUTTON_ID_TRACK_4_EDIT),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_4_PLAY_RECORD,                          \
                             BUTTON_ID_TRACK_4_PLAY_RECORD),                                       \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_4_STOP, BUTTON_ID_TRACK_4_STOP),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_5_EDIT, BUTTON_ID_TRACK_5_EDIT),        \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_5_PLAY_RECORD,                          \
                             BUTTON_ID_TRACK_5_PLAY_RECORD),                                       \
        ConfigTable_1D_ENTRY(MCP23017_GPIO_ID_BUTTON_TRACK_5_STOP, BUTTON_ID_TRACK_5_STOP),

ConfigTable_1D_DECLARE_TABLE(Mcp23017GpioId, ButtonId, MCP23017_GPIO_ID_COUNT, ENTRIES);
#undef ENTRIES
// ConfigValidator_REGISTER(&gpio_pin_button_config_map, Mcp23017GpioId, ButtonId);

ButtonId Mcp23017GpioMap_Get(Mcp23017GpioId gpio_id)
{
    if (gpio_id <= MCP23017_GPIO_ID_NONE || gpio_id >= MCP23017_GPIO_ID_COUNT) {
        return MCP23017_GPIO_ID_NONE;
    }
    return ConfigTable_1D_GET(Mcp23017GpioId, ButtonId, gpio_id);
}
