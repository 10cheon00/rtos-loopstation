#ifndef UI_TRANSITION_MAP_HPP
#define UI_TRANSITION_MAP_HPP

#include <stddef.h>

#include "button_id.hpp"
#include "ui_state_id.hpp"

namespace UiTransitionMap {

UiStateMachine::Id Get(ButtonId ui_action_id);

}  // namespace UiTransitionMap

#endif
