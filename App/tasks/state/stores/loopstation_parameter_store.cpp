#include "loopstation_parameter_store.hpp"

#include <cstddef>

namespace LoopstationStore {

/**
 * 파라미터 초기값 설정은 initial_parameters에서 하고, 변경 가능한 EnumMap을
 * 제공하기 위해 이를 복사한 actual_parameters를 선언한다.
 */
static constexpr ParameterStore initial_parameters{
    EnumEntry{ParameterModel::Id::NONE, Parameter{0, 0, 0, ParameterType::NONE}},
    EnumEntry{ParameterModel::Id::IFX_KNOB,
              Parameter{0, 100, 0, ParameterType::UNSIGNED_RANGE}},
    EnumEntry{ParameterModel::Id::TFX_KNOB,
              Parameter{0, 100, 0, ParameterType::UNSIGNED_RANGE}},
    EnumEntry{ParameterModel::Id::IFX_A_STATE,
              Parameter{0, 1, 0, ParameterType::TOGGLE}},
    EnumEntry{ParameterModel::Id::TFX_A_STATE,
              Parameter{0, 1, 0, ParameterType::TOGGLE}},
    EnumEntry{ParameterModel::Id::SYSTEM_SETTING_LCD_CONSTRAST,
              Parameter{10, 80, 80, ParameterType::UNSIGNED_RANGE}},
    EnumEntry{ParameterModel::Id::TRACK_1_VOLUME,
              Parameter{0, 100, 0, ParameterType::UNSIGNED_RANGE}},
};

static ParameterStore actual_parameters{initial_parameters};

Parameter& GetParameter(ParameterModel::Id parameter_id) {
  return actual_parameters.Get(parameter_id);
}

}  // namespace LoopstationStore
