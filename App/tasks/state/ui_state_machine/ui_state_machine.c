#include "ui_state_machine.h"

#include "ui_panel_id_ui_state_mapping.h"

void UiStateMachine_Init(UiStateMachine *ui_state_machine, UiStateMachineContext *context,
                         UiState *init_state)
{
    ui_state_machine->context = context;
    ui_state_machine->current_state = init_state;
}

void UiStateMachine_TryTransition(UiStateMachine *ui_state_machine, UiStateEventId ui_state_event_id) {

}

void UiStateMachineContext_Init(UiStateMachineContext *ui_state_machine_context,
                                osMessageQueueId_t display_command_queue)
{
    ui_state_machine_context->display_command_queue = display_command_queue;   
}

void UiStateMachine_RenderCurrentState(UiStateMachine *ui_state_machine) {

}
