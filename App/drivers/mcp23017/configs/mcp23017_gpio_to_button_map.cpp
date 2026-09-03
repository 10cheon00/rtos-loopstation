#include "mcp23017_gpio_to_button_map.hpp"

#include "button_id.hpp"
#include "enum_map.hpp"

/**
 * Mcp23017GpioId는 ButtonId와 유사하지만 입력 + 출력용 Gpio들을 모두 모은
 * 것이다. 따라서 입력용 Gpio들을 ButtonId로 변환시켜주려면 테이블이 필요하다.
 * 이 테이블은 수동으로 컨버팅한다.
 */

using namespace Mcp23017;

namespace Mcp23017GpioToButtonMap {

static constexpr EnumMap<GpioId, ButtonId> gpio_to_button_map{
    EnumEntry{GpioId::NONE, ButtonId::NONE},
    EnumEntry{GpioId::LED_IFX_A, ButtonId::NONE},
    EnumEntry{GpioId::LED_TFX_A, ButtonId::NONE},
    EnumEntry{GpioId::BUTTON_LEFT, ButtonId::LEFT},
    EnumEntry{GpioId::BUTTON_RIGHT, ButtonId::RIGHT},
    EnumEntry{GpioId::BUTTON_ENTER, ButtonId::ENTER},
    EnumEntry{GpioId::BUTTON_EXIT, ButtonId::EXIT},
    EnumEntry{GpioId::BUTTON_ENCODER_A_PUSH, ButtonId::ENCODER_A_PUSH},
    EnumEntry{GpioId::BUTTON_ENCODER_B_PUSH, ButtonId::ENCODER_B_PUSH},
    EnumEntry{GpioId::BUTTON_ENCODER_C_PUSH, ButtonId::ENCODER_C_PUSH},
    EnumEntry{GpioId::BUTTON_ENCODER_D_PUSH, ButtonId::ENCODER_D_PUSH},
    EnumEntry{GpioId::BUTTON_IFX_A_TOGGLE, ButtonId::IFX_A_TOGGLE},
    EnumEntry{GpioId::BUTTON_TFX_A_TOGGLE, ButtonId::TFX_A_TOGGLE},
    EnumEntry{GpioId::BUTTON_TRACK_1_EDIT, ButtonId::TRACK_1_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_1_PLAY_RECORD,
              ButtonId::TRACK_1_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_1_STOP, ButtonId::TRACK_1_STOP},
    EnumEntry{GpioId::BUTTON_TRACK_2_EDIT, ButtonId::TRACK_2_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_2_PLAY_RECORD,
              ButtonId::TRACK_2_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_2_STOP, ButtonId::TRACK_2_STOP},
    EnumEntry{GpioId::BUTTON_TRACK_3_EDIT, ButtonId::TRACK_3_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_3_PLAY_RECORD,
              ButtonId::TRACK_3_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_3_STOP, ButtonId::TRACK_3_STOP},
    EnumEntry{GpioId::BUTTON_TRACK_4_EDIT, ButtonId::TRACK_4_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_4_PLAY_RECORD,
              ButtonId::TRACK_4_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_4_STOP, ButtonId::TRACK_4_STOP},
    EnumEntry{GpioId::BUTTON_TRACK_5_EDIT, ButtonId::TRACK_5_EDIT},
    EnumEntry{GpioId::BUTTON_TRACK_5_PLAY_RECORD,
              ButtonId::TRACK_5_PLAY_RECORD},
    EnumEntry{GpioId::BUTTON_TRACK_5_STOP, ButtonId::TRACK_5_STOP},
    EnumEntry{GpioId::LED_TRACK_1_RED, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_1_GREEN, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_1_BLUE, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_2_RED, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_2_GREEN, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_2_BLUE, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_3_RED, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_3_GREEN, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_3_BLUE, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_4_RED, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_4_GREEN, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_4_BLUE, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_5_RED, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_5_GREEN, ButtonId::NONE},
    EnumEntry{GpioId::LED_TRACK_5_BLUE, ButtonId::NONE},
};

ButtonId Get(GpioId gpio_id) { return gpio_to_button_map[gpio_id]; }

}  // namespace Mcp23017GpioToButtonMap
