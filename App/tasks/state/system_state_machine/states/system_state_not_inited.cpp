#include "system_state_not_inited.h"

#include <array>

#include "mcp23017.hpp"
#include "config_validator.h"

static constexpr auto system_state_not_init_transition_table = [] {
    std::array<SystemStateId, SYSTEM_ACTION_ID_COUNT> values{};
    values[SYSTEM_ACTION_ID_INITED] = SYSTEM_STATE_ID_RUNNING;
    values[SYSTEM_ACTION_ID_ERROR] = SYSTEM_STATE_ID_ERROR;
    return values;
}();

static SystemStateOnEnterResult SystemStateNotInited_OnEnter(SystemStateMachineContext *context);

SystemState SYSTEM_STATE_NOT_INITED = {
    .id = SYSTEM_STATE_ID_NOT_INITED,
    .transition_table = system_state_not_init_transition_table.data(),
    .OnEnter = SystemStateNotInited_OnEnter,
};

static SystemStateOnEnterResult SystemStateNotInited_OnEnter(SystemStateMachineContext *context)
{
    // ConfigValidatorResult result = ConfigValidator_Validate();
    // if (result != CONFIG_VALIDATOR_RESULT_OK) {
    //     return (SystemStateOnEnterResult){.action_id = SYSTEM_ACTION_ID_ERROR,
    //                                       .is_transition_requested = true};
    // }

    // TODO:
    // Mcp23017Driver 초기화 수행
    return (SystemStateOnEnterResult){.action_id = SYSTEM_ACTION_ID_INITED,
                                      .is_transition_requested = true};
}
