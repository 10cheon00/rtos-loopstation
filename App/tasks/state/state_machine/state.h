#ifndef STATE_H
#define STATE_H

#include "state_messages.h"

typedef struct State State;

typedef uint32_t StateOnEventResultFlags;

#define STATE_ON_EVENT_HANDLING_FLAG_ERROR (0x0)
#define STATE_ON_EVENT_HANDLING_FLAG_REJECTED (0x1)
#define STATE_ON_EVENT_HANDLING_FLAG_IGNORED (0x2)
#define STATE_ON_EVENT_HANDLING_FLAG_PARAMETER_UPDATED (0x4)
#define STATE_ON_EVENT_HANDLING_FLAG_TRANSITION (0x8)

typedef void (*StateOnEnterFunction)(void *context);
typedef StateOnEventResultFlags (*StateOnEventFunction)(const StateEvent *event, State *next_state);
typedef void (*StateOnExitFunction)(void);

struct State {
    StateOnEnterFunction on_enter;
    StateOnEventFunction on_event;
    StateOnExitFunction on_exit;
    void *context;
    const uint16_t id; // TODO: enum으로 나타낸 상태 ID를 uint16_t로 취급해도 되는지 결정하기
};

#endif
