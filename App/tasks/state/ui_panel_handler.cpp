#include "ui_panel_handler.hpp"

#include "ui_state_navigation_tree.hpp"
#include "ui_transition_map.hpp"
#include "utils.h"

void UiPanelHandler::Handle(ButtonEvent& button_event) {
  TryChangePageIndex(button_event.id);
  TryTransitionUiStateMachine(button_event.id);
}

void UiPanelHandler::TryChangePageIndex(ButtonId id) {
  if (id == ButtonId::LEFT) {
    state_task_context.ui_state_machine.GetCurrentState()->IncreasePageIndex();
  } else if (id == ButtonId::RIGHT) {
    state_task_context.ui_state_machine.GetCurrentState()->DecreasePageIndex();
  }
}

void UiPanelHandler::TryTransitionUiStateMachine(ButtonId id) {
  UiStateMachine::Id next_ui_state_id = UiStateMachine::Id::NONE;

  switch (id) {
    case ButtonId::EXIT:
      next_ui_state_id = UiStateNavigationTree_GetParent(
          state_task_context.ui_state_machine.GetCurrentState()->GetId());
      break;
    case ButtonId::ENCODER_A_PUSH:
    case ButtonId::ENCODER_B_PUSH:
    case ButtonId::ENCODER_C_PUSH:
    case ButtonId::ENCODER_D_PUSH: {
      SlotIndex index = ToSlotIndex(id);
      PageSlotVariant& page_slot_variant =
          state_task_context.ui_state_machine.GetCurrentState()
              ->GetCurrentPage()
              .GetAt(index);
      if (std::holds_alternative<MenuSlot>(page_slot_variant)) {
        next_ui_state_id = std::get<MenuSlot>(page_slot_variant).GetUiStateId();
      }
    } break;
    default:
      next_ui_state_id = UiTransitionMap::Get(id);
  }
  if (next_ui_state_id != UiStateMachine::Id::NONE) {
    state_task_context.ui_state_machine.TryTransition(next_ui_state_id);
  }
}
