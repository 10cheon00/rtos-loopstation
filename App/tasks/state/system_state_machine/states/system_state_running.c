#include "system_state_running.h"

static SystemStateId system_state_running_transition_table[SYSTEM_ACTION_ID_COUNT] = {
    [SYSTEM_ACTION_ID_ERROR] = SYSTEM_STATE_ID_ERROR,
};

static SystemStateOnEnterResult SystemStateRunning_OnEnter(SystemStateMachineContext *context);

SystemState SYSTEM_STATE_RUNNING = {
    .id = SYSTEM_STATE_ID_RUNNING,
    .transition_table = system_state_running_transition_table,
    .OnEnter = SystemStateRunning_OnEnter,
};

static SystemStateOnEnterResult SystemStateRunning_OnEnter(SystemStateMachineContext *context)
{
    return (SystemStateOnEnterResult){.action_id = SYSTEM_ACTION_ID_NONE,
                                      .is_transition_requested = false};
}
