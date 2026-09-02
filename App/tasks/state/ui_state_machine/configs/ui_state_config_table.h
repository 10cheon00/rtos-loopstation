#ifndef UI_STATE_CONFIG_TABLE_H
#define UI_STATE_CONFIG_TABLE_H

#include "ui_state.h"
#include "ui_state_id.h"

typedef UiState* UiStatePointer;

UiStatePointer UiStateConfigTable_Get(UiStateId ui_state_id);

#endif
