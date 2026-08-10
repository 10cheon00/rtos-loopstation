#include "knob_parameter_config_map.h"

#include "config_map.h"
#include "config_validator.h"

static ConfigMapEntry knob_parameter_config_map_entries[] = {
    {.key = KNOB_ID_NONE, .value = PARAMETER_ID_NONE},
    {.key = KNOB_ID_IFX, .value = PARAMETER_ID_IFX_KNOB},
    {.key = KNOB_ID_TFX, .value = PARAMETER_ID_TFX_KNOB},
    {.key = KNOB_ID_TRACK_1_VOLUME_FADER, .value = PARAMETER_ID_TRACK_1_VOLUME},
};

static ConfigMap knob_parameter_config_map = {.entries = knob_parameter_config_map_entries,
                                              .count =
                                                  ARRAY_COUNT(knob_parameter_config_map_entries)};

ConfigValidator_REGISTER(&knob_parameter_config_map, KnobId, ParameterId);

ParameterId KnobParameterConfigMap_Get(KnobId knob_id)
{
    Value_t value;
    if (ConfigMap_Get(&knob_parameter_config_map, knob_id, &value) != CONFIG_MAP_RESULT_OK) {
        return PARAMETER_ID_NONE;
    }
    return (ParameterId)value;
}
