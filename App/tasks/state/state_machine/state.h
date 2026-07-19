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
};

#endif