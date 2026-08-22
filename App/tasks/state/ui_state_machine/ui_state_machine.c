#include "ui_state_machine.h"

#include "display_messages.h"
#include "ui_state_config_table.h"
#include "loopstation_parameter_store.h"

void UiStateMachine_Init(UiStateMachine *ui_state_machine, UiStateMachineContext *context,
                         UiState *init_state)
{
    ui_state_machine->context = context;
    ui_state_machine->current_state = init_state;
}

void UiStateMachine_TryTransition(UiStateMachine *ui_state_machine, UiActionId ui_action_id)
{
    UiStateId next_ui_state_id;
    UiState *next_ui_state;

    next_ui_state_id =
        UiState_GetNextUiStateId(ui_state_machine->current_state, ui_action_id);
    next_ui_state = UiStateConfigTable_Get(next_ui_state_id);
    if (next_ui_state != NULL) {
        ui_state_machine->current_state = next_ui_state;
    }
    // TODO:
    // 전이가 안 된 경우에 대한 반환값 추가하기
}

void UiStateMachineContext_Init(UiStateMachineContext *ui_state_machine_context) {}
