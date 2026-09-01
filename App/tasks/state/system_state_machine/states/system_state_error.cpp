#include "system_state_error.h"

static SystemStateOnEnterResult SystemStateError_OnEnter(SystemStateMachineContext *context);

static SystemStateId system_state_error_transition_table[SYSTEM_ACTION_ID_COUNT] = {};

SystemState SYSTEM_STATE_ERROR = {
    .id = SYSTEM_STATE_ID_ERROR,
    .transition_table = system_state_error_transition_table,
    .OnEnter = SystemStateError_OnEnter,
};

static SystemStateOnEnterResult SystemStateError_OnEnter(SystemStateMachineContext *context)
{
    return (SystemStateOnEnterResult){.action_id = SYSTEM_ACTION_ID_NONE,
                                      .is_transition_requested = false};
}
