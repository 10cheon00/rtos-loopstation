#ifndef UI_STATE_MACHINE_H
#define UI_STATE_MACHINE_H

#include "cmsis_os2.h"
#include "ui_state.h"

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
