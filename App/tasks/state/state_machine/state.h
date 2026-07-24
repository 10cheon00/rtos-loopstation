#ifndef STATE_H
#define STATE_H

#include "state_messages.h"

typedef struct State State;

typedef enum {
    EVENT_HANDLING_STATUS_HANDLED,
    EVENT_HANDLING_STATUS_TRANSITION,
    EVENT_HANDLING_STATUS_IGNORED,
    EVENT_HANDLING_STATUS_REJECTED,
    EVENT_HANDLING_STATUS_ERROR
} EventHandlingStatus;

typedef struct {
    const State *next_state;
    EventHandlingStatus status;
} EventHandlingResult;

typedef void(*OnEnterFunction)(void* context);
typedef EventHandlingResult(*OnEventFunction)(const StateEvent* event);
typedef void(*OnExitFunction)(void);

struct State {
    OnEnterFunction on_enter;
    OnEventFunction on_event;
    OnExitFunction on_exit;
    void *context;
    const uint16_t id; // TODO: enum으로 나타낸 상태 ID를 uint16_t로 취급해도 되는지 결정하기
};

#endif