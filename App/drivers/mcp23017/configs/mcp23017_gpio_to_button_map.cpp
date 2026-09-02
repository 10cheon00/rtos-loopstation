#include "mcp23017_gpio_to_button_map.hpp"

#include "enum_map.hpp"

/**
 * Mcp23017GpioId는 ButtonId와 유사하지만 입력 + 출력용 Gpio들을 모두 모은
 * 것이다. 따라서 입력용 Gpio들을 ButtonId로 변환시켜주려면 테이블이 필요하다.
 * 이 테이블은 수동으로 컨버팅한다.
 */

using namespace Mcp23017;

namespace Mcp23017GpioToButtonMap {

static constexpr EnumMap<GpioId, ButtonId> gpio_to_button_map{
    EnumEntry{GpioId::NONE, BUTTON_ID_NONE},
    EnumEntry{GpioId::LED_IFX_A, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TFX_A, BUTTON_ID_NULL},
    EnumEntry{GpioId::BUTTON_LEFT, BUTTON_ID_LEFT},
    EnumEntry{GpioId::BUTTON_RIGHT, BUTTON_ID_RIGHT},
    EnumEntry{GpioId::BUTTON_ENTER, BUTTON_ID_ENTER},
    EnumEntry{GpioId::BUTTON_EXIT, BUTTON_ID_EXIT},
    EnumEntry{GpioId::BUTTON_ENCODER_A_PUSH, BUTTON_ID_ENCODER_A_PUSH},
    EnumEntry{GpioId::BUTTON_ENCODER_B_PUSH, BUTTON_ID_ENCODER_B_PUSH},
    EnumEntry{GpioId::BUTTON_ENCODER_C_PUSH, BUTTON_ID_ENCODER_C_PUSH},
    EnumEntry{GpioId::BUTTON_ENCODER_D_PUSH, BUTTON_ID_ENCODER_D_PUSH},
    EnumEntry{GpioId::BUTTON_IFX_A_TOGGLE, BUTTON_ID_IFX_A_TOGGLE},
    EnumEntry{GpioId::BUTTON_TFX_A_TOGGLE, BUTTON_ID_TFX_A_TOGGLE},
    EnumEntry{GpioId::BUTTON_TRACK_1_EDIT, BUTTON_ID_TRACK_1_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_1_PLAY_RECORD,
              BUTTON_ID_TRACK_1_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_1_STOP, BUTTON_ID_TRACK_1_STOP},
    EnumEntry{GpioId::BUTTON_TRACK_2_EDIT, BUTTON_ID_TRACK_2_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_2_PLAY_RECORD,
              BUTTON_ID_TRACK_2_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_2_STOP, BUTTON_ID_TRACK_2_STOP},
    EnumEntry{GpioId::BUTTON_TRACK_3_EDIT, BUTTON_ID_TRACK_3_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_3_PLAY_RECORD,
              BUTTON_ID_TRACK_3_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_3_STOP, BUTTON_ID_TRACK_3_STOP},
    EnumEntry{GpioId::BUTTON_TRACK_4_EDIT, BUTTON_ID_TRACK_4_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_4_PLAY_RECORD,
              BUTTON_ID_TRACK_4_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_4_STOP, BUTTON_ID_TRACK_4_STOP},
    EnumEntry{GpioId::BUTTON_TRACK_5_EDIT, BUTTON_ID_TRACK_5_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_5_PLAY_RECORD,
              BUTTON_ID_TRACK_5_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_5_STOP, BUTTON_ID_TRACK_5_STOP},
    EnumEntry{GpioId::LED_TRACK_1_RED, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_1_GREEN, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_1_BLUE, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_2_RED, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_2_GREEN, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_2_BLUE, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_3_RED, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_3_GREEN, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_3_BLUE, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_4_RED, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_4_GREEN, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_4_BLUE, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_5_RED, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_5_GREEN, BUTTON_ID_NULL},
    EnumEntry{GpioId::LED_TRACK_5_BLUE, BUTTON_ID_NULL},
};

ButtonId Get(GpioId gpio_id) { return gpio_to_button_map[gpio_id]; }

}  // namespace Mcp23017GpioToButtonMap
