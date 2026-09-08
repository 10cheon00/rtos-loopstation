#ifndef UI_STATE_POINTER_MAP_HPP
#define UI_STATE_POINTER_MAP_HPP

#include "ui_state.hpp"
#include "ui_state_id.hpp"

namespace UiStatePointerMap {

using namespace UiStateMachine;

using UiStatePointer = State*;

UiStatePointer Get(Id ui_state_id);

}  // namespace UiStatePointerMap

#endif
