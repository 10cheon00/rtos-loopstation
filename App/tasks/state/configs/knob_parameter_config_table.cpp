#include "knob_parameter_config_table.h"

#include "enum_map.hpp"

namespace KnobParameterMap {

static constexpr EnumMap<KnobId, ParameterId> knob_parameter_map{
    EnumEntry{KnobId::NONE, PARAMETER_ID_NONE},
    EnumEntry{KnobId::IFX, PARAMETER_ID_IFX_KNOB},
    EnumEntry{KnobId::TFX, PARAMETER_ID_TFX_KNOB},
    EnumEntry{KnobId::TRACK_1_VOLUME_FADER, PARAMETER_ID_TRACK_1_VOLUME},
    EnumEntry{KnobId::TRACK_2_VOLUME_FADER, PARAMETER_ID_NULL},
    EnumEntry{KnobId::TRACK_3_VOLUME_FADER, PARAMETER_ID_NULL},
    EnumEntry{KnobId::TRACK_4_VOLUME_FADER, PARAMETER_ID_NULL},
    EnumEntry{KnobId::TRACK_5_VOLUME_FADER, PARAMETER_ID_NULL},
};

const EnumMap<KnobId, ParameterId>& GetKnobParameterMap() {
  return knob_parameter_map;
}

}