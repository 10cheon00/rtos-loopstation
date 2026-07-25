#ifndef STATE_H
#define STATE_H

#include "state_messages.h"

typedef struct State State;

typedef enum {
    STATE_ON_EVENT_HANDLING_STATUS_ERROR = 0x0,
    STATE_ON_EVENT_HANDLING_STATUS_REJECTED = 0x1,
    STATE_ON_EVENT_HANDLING_STATUS_IGNORED = 0x2,
    STATE_ON_EVENT_HANDLING_STATUS_HANDLED = 0x4,
    STATE_ON_EVENT_HANDLING_STATUS_TRANSITION = 0x8,
} StateOnEventHandlingStatus;

typedef struct {
    const State *next_state;
    StateOnEventHandlingStatus status;
} StateOnEventHandlingResult;

typedef void(*StateOnEnterFunction)(void* context);
typedef StateOnEventHandlingResult(*StateOnEventFunction)(const StateEvent* event);
typedef void(*StateOnExitFunction)(void);

struct State {
    StateOnEnterFunction on_enter;
    StateOnEventFunction on_event;
    StateOnExitFunction on_exit;
    void *context;
    const uint16_t id; // TODO: enum으로 나타낸 상태 ID를 uint16_t로 취급해도 되는지 결정하기
};

#endif