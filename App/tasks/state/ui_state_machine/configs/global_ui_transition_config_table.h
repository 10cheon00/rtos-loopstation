#ifndef GLOBAL_UI_TRANSITION_CONFIG_TABLE_H
#define GLOBAL_UI_TRANSITION_CONFIG_TABLE_H

#include <stddef.h>

#include "button_id.hpp"
#include "ui_state_id.h"

UiStateMachine::Id GlobalUiTransitionConfigTable_Get(ButtonId ui_action_id);

#endif
