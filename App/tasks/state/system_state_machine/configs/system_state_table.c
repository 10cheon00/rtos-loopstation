#include "system_state_table.h"

#include "system_state_not_inited.h"
#include "system_state_running.h"
#include "system_state_error.h"

static SystemState *system_state_table[SYSTEM_STATE_ID_COUNT] = {
    [SYSTEM_STATE_ID_NONE] = NULL,
    [SYSTEM_STATE_ID_NOT_INITED] = &SYSTEM_STATE_NOT_INITED,
    [SYSTEM_STATE_ID_RUNNING] = &SYSTEM_STATE_RUNNING,
    [SYSTEM_STATE_ID_ERROR] = &SYSTEM_STATE_ERROR,
};

SystemState *SystemStateTable_GetSystemState(SystemStateId id)
{
    if (id <= SYSTEM_STATE_ID_NONE || id >= SYSTEM_STATE_ID_COUNT) {
        id = SYSTEM_STATE_ID_NONE;
    }
    return system_state_table[id];
}
