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
    {GpioId::NONE, ButtonId::NONE},
    {GpioId::LED_IFX_A, ButtonId::NONE},
    {GpioId::LED_TFX_A, ButtonId::NONE},
    {GpioId::BUTTON_LEFT, ButtonId::LEFT},
    {GpioId::BUTTON_RIGHT, ButtonId::RIGHT},
    {GpioId::BUTTON_ENTER, ButtonId::ENTER},
    {GpioId::BUTTON_EXIT, ButtonId::EXIT},
    {GpioId::BUTTON_SETTING, ButtonId::SETTING},
    {GpioId::BUTTON_LOOP, ButtonId::LOOP},
    {GpioId::BUTTON_ENCODER_A_PUSH, ButtonId::ENCODER_A_PUSH},
    {GpioId::BUTTON_ENCODER_B_PUSH, ButtonId::ENCODER_B_PUSH},
    {GpioId::BUTTON_ENCODER_C_PUSH, ButtonId::ENCODER_C_PUSH},
    {GpioId::BUTTON_ENCODER_D_PUSH, ButtonId::ENCODER_D_PUSH},
    {GpioId::BUTTON_IFX_A_TOGGLE, ButtonId::IFX_A_TOGGLE},
    {GpioId::BUTTON_TFX_A_TOGGLE, ButtonId::TFX_A_TOGGLE},
    {GpioId::BUTTON_IFX_B_TOGGLE, ButtonId::IFX_B_TOGGLE},
    {GpioId::BUTTON_TFX_B_TOGGLE, ButtonId::TFX_B_TOGGLE},
    {GpioId::BUTTON_IFX_C_TOGGLE, ButtonId::IFX_C_TOGGLE},
    {GpioId::BUTTON_TFX_C_TOGGLE, ButtonId::TFX_C_TOGGLE},
    {GpioId::BUTTON_TRACK_1_EDIT, ButtonId::TRACK_1_EDIT},
    {GpioId::BUTTON_TRACK_1_PLAY_RECORD,
     ButtonId::TRACK_1_PLAY_RECORD},
    {GpioId::BUTTON_TRACK_1_STOP, ButtonId::TRACK_1_STOP},
    {GpioId::BUTTON_TRACK_2_EDIT, ButtonId::TRACK_2_EDIT},
    {GpioId::BUTTON_TRACK_2_PLAY_RECORD,
     ButtonId::TRACK_2_PLAY_RECORD},
    {GpioId::BUTTON_TRACK_2_STOP, ButtonId::TRACK_2_STOP},
    {GpioId::BUTTON_TRACK_3_EDIT, ButtonId::TRACK_3_EDIT},
    {GpioId::BUTTON_TRACK_3_PLAY_RECORD,
     ButtonId::TRACK_3_PLAY_RECORD},
    {GpioId::BUTTON_TRACK_3_STOP, ButtonId::TRACK_3_STOP},
    {GpioId::BUTTON_TRACK_4_EDIT, ButtonId::TRACK_4_EDIT},
    {GpioId::BUTTON_TRACK_4_PLAY_RECORD,
     ButtonId::TRACK_4_PLAY_RECORD},
    {GpioId::BUTTON_TRACK_4_STOP, ButtonId::TRACK_4_STOP},
    {GpioId::BUTTON_TRACK_5_EDIT, ButtonId::TRACK_5_EDIT},
    {GpioId::BUTTON_TRACK_5_PLAY_RECORD,
     ButtonId::TRACK_5_PLAY_RECORD},
    {GpioId::BUTTON_TRACK_5_STOP, ButtonId::TRACK_5_STOP},
};

ButtonId Get(GpioId gpio_id) { return gpio_to_button_map[gpio_id]; }

}  // namespace Mcp23017GpioToButtonMap
