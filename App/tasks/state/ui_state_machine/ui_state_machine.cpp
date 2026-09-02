#include "ui_state_machine.h"

#include "display_messages.h"
#include "ui_state_config_table.h"

void UiStateMachine_Init(UiStateMachine* ui_state_machine,
                         UiStateMachineContext* context, UiState* init_state) {
  ui_state_machine->context = context;
  ui_state_machine->current_state = init_state;
}

void UiStateMachine_TryTransition(UiStateMachine* ui_state_machine,
                                  UiStateId next_ui_state_id) {
  UiState* next_ui_state;

  /**
   * 전이의 경우는 두 가지
   * 전역으로 이동하는 전이 -> UI_ACTION_ID를 전달할 필요 없이 바로 UiState*를
   * 얻어와 전이 상위 패널로 이동하는 전이 -> 어떤 자료구조를 통해 상위
   * UiState*를 얻어와 전이
   */
  if (next_ui_state_id == UI_STATE_ID_NONE) {
    return;
  }
  next_ui_state = UiStateConfigTable_Get(next_ui_state_id);
  if (next_ui_state != NULL) {
    ui_state_machine->current_state = next_ui_state;
  }
}

void UiStateMachineContext_Init(
    UiStateMachineContext* ui_state_machine_context) {}
