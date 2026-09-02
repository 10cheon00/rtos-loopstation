#include "knob_parameter_config_table.h"

#include "config_table.h"
#include "config_validator.h"

#define ENTRIES                                                              \
  ConfigTable_1D_ENTRY(KNOB_ID_NONE, PARAMETER_ID_NONE),                     \
      ConfigTable_1D_ENTRY(KNOB_ID_NULL, PARAMETER_ID_NULL),                 \
      ConfigTable_1D_ENTRY(KNOB_ID_IFX, PARAMETER_ID_IFX_KNOB),              \
      ConfigTable_1D_ENTRY(KNOB_ID_TFX, PARAMETER_ID_TFX_KNOB),              \
      ConfigTable_1D_ENTRY(KNOB_ID_TRACK_1_VOLUME_FADER,                     \
                           PARAMETER_ID_TRACK_1_VOLUME),                     \
      ConfigTable_1D_ENTRY(KNOB_ID_TRACK_2_VOLUME_FADER, PARAMETER_ID_NULL), \
      ConfigTable_1D_ENTRY(KNOB_ID_TRACK_3_VOLUME_FADER, PARAMETER_ID_NULL), \
      ConfigTable_1D_ENTRY(KNOB_ID_TRACK_4_VOLUME_FADER, PARAMETER_ID_NULL), \
      ConfigTable_1D_ENTRY(KNOB_ID_TRACK_5_VOLUME_FADER, PARAMETER_ID_NULL)

ConfigTable_1D_DECLARE_TABLE(KnobId, ParameterId, PARAMETER_ID_COUNT, ENTRIES);
#undef ENTRIES

ConfigValidator_REGISTER_CONFIG_TABLE_1D(KnobId, ParameterId, KNOB_ID_COUNT,
                                         PARAMETER_ID_NONE, PARAMETER_ID_COUNT,
                                         CONFIG_TABLE_TYPE_ALLOW_NULL_VALUE);

ParameterId KnobParameterConfigMap_Get(KnobId knob_id) {
  if (knob_id <= KNOB_ID_NONE || knob_id >= KNOB_ID_COUNT) {
    return PARAMETER_ID_NONE;
  }
  return ConfigTable_1D_GET(KnobId, ParameterId, knob_id);
}
