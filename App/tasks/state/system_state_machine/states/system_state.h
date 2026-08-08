#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <stddef.h>
#include <stdbool.h>

#include "state_id.h"
#include "system_state_machine_context.h"

typedef StateId SystemStateId;

enum SystemStateId {
    SYSTEM_STATE_ID_NONE = 0,
    SYSTEM_STATE_ID_NOT_INITED,
    SYSTEM_STATE_ID_RUNNING,
    SYSTEM_STATE_ID_ERROR,
    SYSTEM_STATE_ID_COUNT,
};

typedef enum {
    SYSTEM_ACTION_ID_NONE = 0,
    SYSTEM_ACTION_ID_INITED,
    SYSTEM_ACTION_ID_ERROR,
    SYSTEM_ACTION_ID_COUNT,
} SystemActionId;

typedef struct {
    SystemActionId action_id;
    bool is_transition_requested;
} SystemStateOnEnterResult;

typedef SystemStateOnEnterResult (*SystemStateOnEnterFunction)(SystemStateMachineContext *);

typedef struct PredefineSystemState {
    SystemStateId id;
    SystemStateId* transition_table;
    SystemStateOnEnterFunction OnEnter;
} SystemState;

SystemStateId SystemState_GetNextSystemStateId(SystemState *state, SystemActionId action_id);

#endif
