#include "loopstation_parameter_store.h"

static LoopStationParameterStore loopstation_parameter_store = {
    .parameters = {
        [PARAMETER_ID_NONE] = {.min = 0, .max = 0, .current = 0},
        [PARAMETER_ID_SETTING_PANEL_CURSOR] =
            {
                .min = 0,
                .max = 1,
                .current = 0,
            },
    }};

LoopStationParameterStore *LoopstationParameterStore_GetInstance()
{
    return &loopstation_parameter_store;
}
