#ifndef UI_STATE_MACHINE_H
#define UI_STATE_MACHINE_H

#include "cmsis_os2.h"

#include "ui_state.h"

typedef struct {
} UiStateMachineContext;

void UiStateMachineContext_Init(UiStateMachineContext *ui_state_machine_context);

struct UiStateMachine {
    UiState *current_state;
    UiStateMachineContext *context;
};

void UiStateMachine_Init(UiStateMachine *ui_state_machine, UiStateMachineContext *context,
                         UiState *init_state);
void UiStateMachine_TryTransition(UiStateMachine *ui_state_machine, UiStateId next_ui_state_id);

#endif
