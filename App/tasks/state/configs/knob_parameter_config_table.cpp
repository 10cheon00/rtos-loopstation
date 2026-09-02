#include "knob_parameter_config_table.h"

#include "enum_map.hpp"

static constexpr EnumMap<KnobId, ParameterId, KNOB_ID_COUNT> knob_parameter_map{
    EnumEntry{KNOB_ID_NONE, PARAMETER_ID_NONE},
    EnumEntry{KNOB_ID_NULL, PARAMETER_ID_NULL},
    EnumEntry{KNOB_ID_IFX, PARAMETER_ID_IFX_KNOB},
    EnumEntry{KNOB_ID_TFX, PARAMETER_ID_TFX_KNOB},
    EnumEntry{KNOB_ID_TRACK_1_VOLUME_FADER, PARAMETER_ID_TRACK_1_VOLUME},
    EnumEntry{KNOB_ID_TRACK_2_VOLUME_FADER, PARAMETER_ID_NULL},
    EnumEntry{KNOB_ID_TRACK_3_VOLUME_FADER, PARAMETER_ID_NULL},
    EnumEntry{KNOB_ID_TRACK_4_VOLUME_FADER, PARAMETER_ID_NULL},
    EnumEntry{KNOB_ID_TRACK_5_VOLUME_FADER, PARAMETER_ID_NULL},
};

ParameterId KnobParameterConfigMap_Get(KnobId knob_id) {
  if (knob_id <= KNOB_ID_NONE || knob_id >= KNOB_ID_COUNT) {
    return PARAMETER_ID_NONE;
  }
  return knob_parameter_map[knob_id];
}
