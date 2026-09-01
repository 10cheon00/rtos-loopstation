#include "system_state_not_inited.h"

#include "config_validator.h"

static SystemStateId system_state_not_init_transition_table[SYSTEM_ACTION_ID_COUNT] = {
    [SYSTEM_ACTION_ID_INITED] = SYSTEM_STATE_ID_RUNNING,
    [SYSTEM_ACTION_ID_ERROR] = SYSTEM_STATE_ID_ERROR,
};

static SystemStateOnEnterResult SystemStateNotInited_OnEnter(SystemStateMachineContext *context);

SystemState SYSTEM_STATE_NOT_INITED = {
    .id = SYSTEM_STATE_ID_NOT_INITED,
    .transition_table = system_state_not_init_transition_table,
    .OnEnter = SystemStateNotInited_OnEnter,
};

static SystemStateOnEnterResult SystemStateNotInited_OnEnter(SystemStateMachineContext *context)
{
    ConfigValidatorResult result = ConfigValidator_Validate();
    if (result != CONFIG_VALIDATOR_RESULT_OK) {
        return (SystemStateOnEnterResult){.action_id = SYSTEM_ACTION_ID_ERROR,
                                          .is_transition_requested = true};
    }
    return (SystemStateOnEnterResult){.action_id = SYSTEM_ACTION_ID_INITED,
                                      .is_transition_requested = true};
}
