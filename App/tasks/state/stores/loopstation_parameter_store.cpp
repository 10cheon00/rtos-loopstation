#include "loopstation_parameter_store.hpp"

#include <cstddef>

namespace LoopstationStore {

using namespace ParameterModel;

/**
 * 파라미터 초기값 설정은 initial_parameters에서 하고, 변경 가능한 EnumMap을
 * 제공하기 위해 이를 복사한 actual_parameters를 선언한다.
 */
static constexpr ParameterStore initial_parameters{
    EnumEntry{
        Id::NONE,
        ValueVariant(),
    },
    EnumEntry{
        Id::IFX_KNOB,
        ValueVariant(UnsignedRangeValue(0, 100, 100)),
    },
    EnumEntry{
        Id::TFX_KNOB,
        ValueVariant(UnsignedRangeValue(0, 100, 100)),
    },
    EnumEntry{
        Id::IFX_A_STATE,
        ValueVariant(ToggleValue(false)),
    },
    EnumEntry{
        Id::TFX_A_STATE,
        ValueVariant(ToggleValue(false)),
    },
    EnumEntry{
        Id::SYSTEM_SETTING_LCD_CONSTRAST,
        ValueVariant(UnsignedRangeValue(10, 80, 80)),
    },
    EnumEntry{
        Id::TRACK_1_VOLUME,
        ValueVariant(UnsignedRangeValue(0, 100, 100)),
    },
};

static ParameterStore actual_parameters{initial_parameters};

ParameterModel::Parameter& GetParameter(ParameterModel::Id parameter_id) {
  return actual_parameters.Get(parameter_id);
}

}  // namespace LoopstationStore
