#ifndef STATE_H
#define STATE_H

#include "state_id.h"
#include "state_messages.h"

typedef struct State State;

typedef uint32_t StateOnEventResultFlags;

#define STATE_ON_EVENT_HANDLING_FLAG_ERROR (0x0)
#define STATE_ON_EVENT_HANDLING_FLAG_REJECTED (0x1)
#define STATE_ON_EVENT_HANDLING_FLAG_IGNORED (0x2)
#define STATE_ON_EVENT_HANDLING_FLAG_PARAMETER_UPDATED (0x4)
#define STATE_ON_EVENT_HANDLING_FLAG_TRANSITION (0x8)

typedef void (*StateOnEnterFunction)(void *context);
typedef StateOnEventResultFlags (*StateOnEventFunction)(const StateEvent *event, StateId *next_state_id);
typedef void (*StateOnExitFunction)(void);

struct State {
    StateOnEnterFunction on_enter;
    StateOnEventFunction on_event;
    StateOnExitFunction on_exit;
    void *context;
    const StateId id;
};

#endif
