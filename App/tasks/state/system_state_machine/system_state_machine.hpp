#ifndef SYSTEM_STATE_MACHINE_H
#define SYSTEM_STATE_MACHINE_H

#include "system_state.hpp"
#include "system_state_machine_context.hpp"
#include "system_state_pointer_map.hpp"

namespace SystemStateMachine {

using namespace SystemStatePointerMap;

class StateMachine {
 public:
  StateMachine(Context& context, Id id) : context(context) {
    SystemStatePointer next_system_state = SystemStatePointerMap::Get(id);
    if (next_system_state != nullptr) {
      this->current_state = next_system_state;
    }
  }

  SystemStatePointer GetCurrentState() const { return this->current_state; }
  Context& GetContext() const { return this->context; }

  void TryTransition(ActionId action_id) {
    Id next_state_id = this->current_state->GetNextStateId(action_id);
    State* next_state = Get(next_state_id);
    if (next_state == NULL) {
      // TODO:
      // 전이가 안된 경우에 대해 예외처리 하기
      return;
    }
    this->current_state = next_state;
    this->current_state->OnEnter(this->context);
  }

 private:
  SystemStatePointer current_state;
  Context& context;
};

}  // namespace SystemStateMachine

#endif
