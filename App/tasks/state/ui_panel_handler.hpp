#ifndef UI_PANEL_HANDLER_HPP
#define UI_PANEL_HANDLER_HPP

#include "state_event_variant.hpp"
#include "state_task_context.hpp"

class UiPanelHandler {
 public:
  UiPanelHandler(StateTaskContext& context) : state_task_context(context) {}

  void Handle(ButtonEvent& button_event);

 private:
  StateTaskContext& state_task_context;

  void TryChangePageIndex(ButtonId id);
  void TryTransitionUiStateMachine(ButtonId id);
};

#endif
