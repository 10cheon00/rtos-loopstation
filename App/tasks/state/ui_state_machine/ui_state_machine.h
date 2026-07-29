#ifndef UI_STATE_MACHINE_H
#define UI_STATE_MACHINE_H

#include "cmsis_os2.h"

#include "ui_state.h"

typedef struct {
    osMessageQueueId_t display_command_queue;
} UiStateMachineContext;

struct UiStateMachine {
    UiState *current_state;
    UiStateMachineContext *context;
};

void UiStateMachine_Init(UiStateMachine *ui_state_machine, UiStateMachineContext *context,
                         UiState *init_state);

void UiStateMachineContext_Init(UiStateMachineContext* ui_state_machine_context, osMessageQueueId_t display_command_queue);

#endif
