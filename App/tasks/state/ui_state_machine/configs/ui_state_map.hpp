#ifndef UI_STATE_POINTER_MAP_HPP
#define UI_STATE_POINTER_MAP_HPP

#include "ui_state.hpp"
#include "ui_state_id.hpp"

namespace UiStateMap {

using namespace UiStateMachine;
using StatePointer = State*;

StatePointer Get(Id ui_state_id);

}  // namespace UiStateMap

#endif
