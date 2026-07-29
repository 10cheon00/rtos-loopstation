#ifndef STATE_H
#define STATE_H

#include <stddef.h>

#include "state_id.h"
#include "state_messages.h"

typedef uint32_t StateOnEventResultFlags;

#define STATE_ON_EVENT_HANDLING_FLAG_ERROR (0x0)
#define STATE_ON_EVENT_HANDLING_FLAG_REJECTED (0x1)
#define STATE_ON_EVENT_HANDLING_FLAG_IGNORED (0x2)
#define STATE_ON_EVENT_HANDLING_FLAG_PARAMETER_UPDATED (0x4)
#define STATE_ON_EVENT_HANDLING_FLAG_TRANSITION (0x8)

typedef enum {
    STATE_FUNCTION_STATUS_OK = 0,
    STATE_FUNCTION_STATUS_ERROR,
} StateFunctionStatus;

typedef StateFunctionStatus (*State_OnEnterFunction)(void *context);
typedef StateFunctionStatus (*State_OnEventFunction)(StateEvent *event, void *context);

typedef struct {
    State_OnEnterFunction on_enter;
    State_OnEventFunction on_event;
    StateId id;
} State;

#endif
