#include "loopstation_parameter_store.hpp"

#include <cstddef>

namespace LoopstationStore {

/**
 * 파라미터 초기값 설정은 initial_parameters에서 하고, 변경 가능한 EnumMap을
 * 제공하기 위해 이를 복사한 actual_parameters를 선언한다.
 */
static constexpr ParameterStore initial_parameters{
    EnumEntry{ParameterId::NONE, Parameter{0, 0, 0, PARAMETER_TYPE_SLIDER}},
    EnumEntry{ParameterId::SETTING_PANEL_CURSOR,
              Parameter{0, 100, 0, PARAMETER_TYPE_SLIDER}},
    EnumEntry{ParameterId::TFX_KNOB,
              Parameter{0, 100, 0, PARAMETER_TYPE_SLIDER}},
    EnumEntry{ParameterId::IFX_KNOB,
              Parameter{0, 100, 0, PARAMETER_TYPE_SLIDER}},
    EnumEntry{ParameterId::IFX_A_STATE,
              Parameter{0, 1, 0, PARAMETER_TYPE_TOGGLE}},
    EnumEntry{ParameterId::TFX_A_STATE,
              Parameter{0, 1, 0, PARAMETER_TYPE_TOGGLE}},
    EnumEntry{ParameterId::SYSTEM_SETTING_LCD_CONSTRAST,
              Parameter{10, 80, 80, PARAMETER_TYPE_SLIDER}},
};

static ParameterStore actual_parameters{initial_parameters};

Parameter& GetParameter(ParameterId parameter_id) {
  return actual_parameters.Get(parameter_id);
}

}  // namespace LoopstationStore
