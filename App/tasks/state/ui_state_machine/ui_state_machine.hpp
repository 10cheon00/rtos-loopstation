#ifndef UI_STATE_MACHINE_HPP
#define UI_STATE_MACHINE_HPP

#include "cmsis_os2.h"
#include "ui_state.hpp"

namespace UiStateMachine {

struct Context {};

void InitContext(Context* context);

struct StateMachine {
  State* current_state;
  Context* context;
};

void Init(StateMachine* ui_state_machine, Context* context, State* init_state);
void TryTransition(StateMachine* ui_state_machine, Id next_ui_state_id);

}  // namespace UiStateMachine

#endif
