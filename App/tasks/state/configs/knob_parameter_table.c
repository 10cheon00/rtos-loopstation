#include "knob_parameter_table.h"

#include "utils.h"

static ParameterId knob_paramter_table[] = {
    [KNOB_ID_NONE] = PARAMETER_ID_NONE,
    [KNOB_ID_IFX] = PARAMETER_ID_IFX_KNOB,
    [KNOB_ID_TFX] = PARAMETER_ID_TFX_KNOB,
    [KNOB_ID_TRACK_1_VOLUME_FADER] = PARAMETER_ID_TRACK_1_VOLUME,
};

static size_t knob_paramter_table_count = ARRAY_COUNT(knob_paramter_table);

ParameterId KnobParameterTable_GetParameterIdFromKnobId(KnobId knob_id)
{
    if (knob_id <= KNOB_ID_NONE || knob_id >= KNOB_ID_COUNT) {
        return PARAMETER_ID_NONE;
    }
    return knob_paramter_table[knob_id];
}
