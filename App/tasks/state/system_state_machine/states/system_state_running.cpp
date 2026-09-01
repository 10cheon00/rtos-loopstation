#include "system_state_running.h"

#include <array>

static constexpr auto system_state_running_transition_table = [] {
    std::array<SystemStateId, SYSTEM_ACTION_ID_COUNT> values{};
    values[SYSTEM_ACTION_ID_ERROR] = SYSTEM_STATE_ID_ERROR;
    return values;
}();

static SystemStateOnEnterResult SystemStateRunning_OnEnter(SystemStateMachineContext *context);

SystemState SYSTEM_STATE_RUNNING = {
    .id = SYSTEM_STATE_ID_RUNNING,
    .transition_table = system_state_running_transition_table.data(),
    .OnEnter = SystemStateRunning_OnEnter,
};

static SystemStateOnEnterResult SystemStateRunning_OnEnter(SystemStateMachineContext *context)
{
    return (SystemStateOnEnterResult){.action_id = SYSTEM_ACTION_ID_NONE,
                                      .is_transition_requested = false};
}
