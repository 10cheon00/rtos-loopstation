#include "ui_state_machine.h"

void UiStateMachine_Init(UiStateMachine *ui_state_machine, UiStateMachineContext *context,
                         UiState *init_state)
{
    ui_state_machine->context = context;
    ui_state_machine->current_state = init_state;
    ui_state_machine->current_state->UiState_OnEnter(ui_state_machine);
}

void UiStateMachineContext_Init(UiStateMachineContext *ui_state_machine_context,
                                osMessageQueueId_t display_command_queue)
{
    ui_state_machine_context->display_command_queue = display_command_queue;   
}
