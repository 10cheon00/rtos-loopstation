#include "loopstation_parameter_store.h"

static LoopStationParameterStore loopstation_parameter_store = {
    .parameters = {
        [PARAMETER_ID_NONE] = {.min = 0, .max = 0, .current = 0, .type = PARAMETER_TYPE_SLIDER},
        [PARAMETER_ID_SETTING_PANEL_CURSOR] =
            {
                .min = 0,
                .max = 1,
                .current = 0,
                .type = PARAMETER_TYPE_SLIDER,
            },
    }};

LoopStationParameterStore *LoopstationParameterStore_GetInstance()
{
    return &loopstation_parameter_store;
}

Parameter *LoopStationParameterStore_GetParameterFromId(ParameterId parameter_id)
{
    if (parameter_id <= PARAMETER_ID_NONE || parameter_id >= PARAMETER_ID_COUNT) {
        return &loopstation_parameter_store.parameters[PARAMETER_ID_NONE];
    }
    return &loopstation_parameter_store.parameters[parameter_id];
}
