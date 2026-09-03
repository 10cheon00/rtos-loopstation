#ifndef UI_STATE_CONFIG_TABLE_H
#define UI_STATE_CONFIG_TABLE_H

#include "ui_state.h"
#include "ui_state_id.h"

namespace UiStatePointerMap {

using namespace UiStateMachine;

using UiStatePointer = State*;

UiStatePointer Get(Id ui_state_id);

}  // namespace UiStatePointerMap

#endif
