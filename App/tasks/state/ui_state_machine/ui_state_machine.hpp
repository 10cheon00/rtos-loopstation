#ifndef UI_STATE_MACHINE_HPP
#define UI_STATE_MACHINE_HPP

#include "cmsis_os2.h"
#include "ui_state.hpp"
#include "ui_state_machine_context.hpp"
#include "ui_state_map.hpp"

namespace UiStateMachine {

class StateMachine {
 public:
  StateMachine(Context& context, Id id)
      : current_state(UiStateMap::Get(Id::HOME)), context(context) {}

  UiStateMap::StatePointer GetCurrentState() const { return current_state; }
  Context& GetContext() const { return this->context; }

  void TryTransition(Id next_ui_state_id) {
    /**
     * 전이의 경우는 두 가지
     * 전역으로 이동하는 전이 -> UI_ACTION_ID를 전달할 필요 없이 바로 UiState*를
     * 얻어와 전이 상위 패널로 이동하는 전이 -> 어떤 자료구조를 통해 상위
     * UiState*를 얻어와 전이
     */
    if (next_ui_state_id == Id::NONE) {
      return;
    }
    UiStateMap::StatePointer next_ui_state = UiStateMap::Get(next_ui_state_id);
    this->current_state = next_ui_state;
  }

 private:
  UiStateMap::StatePointer current_state;
  Context& context;
};

}  // namespace UiStateMachine

#endif
