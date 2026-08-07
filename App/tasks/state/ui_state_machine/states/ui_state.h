#ifndef UI_STATE_H
#define UI_STATE_H

#include <stddef.h>

#include "ui_panel_id.h"

typedef enum {
    UI_ACTION_NONE,
    UI_ACTION_NAVIGATE_LEFT,
    UI_ACTION_NAVIGATE_RIGHT,
    UI_ACTION_ENTER,
    UI_ACTION_ENTER_ENCODER_A,
    UI_ACTION_ENTER_ENCODER_B,
    UI_ACTION_ENTER_ENCODER_C,
    UI_ACTION_ENTER_ENCODER_D,
    UI_ACTION_EXIT,
    UI_ACTION_TO_IFX,
    UI_ACTION_TO_TFX,
    UI_ACTION_COUNT
} UiActionId;

/* 순환 참조를 막기 위한 전방 선언 */
typedef struct UiStateMachine UiStateMachine;

typedef struct {
    UiActionId ui_action_id;
    UiPanelId next_ui_panel_id;
} UiTransitionMapEntry;

typedef struct {
    UiTransitionMapEntry* ui_transition_map;
    size_t ui_transition_map_count;
    UiPanelId ui_panel_id;
} UiState;

UiPanelId UiState_GetUiPanelIdFromUiActionId(UiState* ui_state, UiActionId ui_action_id);

#endif
