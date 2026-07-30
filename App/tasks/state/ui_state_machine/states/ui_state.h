#ifndef UI_STATE_H
#define UI_STATE_H

#include <stddef.h>

#include "ui_panel_id.h"

// TODO:
// UiStateEvent는 더 큰 범주를 의미하는 것 같다. 이벤트가 전이 이벤트 하나뿐인가?
typedef enum {
    UI_ACTION_NONE,
    UI_ACTION_NAVIGATE_LEFT,
    UI_ACTION_NAVIGATE_RIGHT,
    UI_ACTION_ENTER,
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
} UiTransitionMapping;

typedef struct {
    UiTransitionMapping* ui_transition_mappings;
    size_t ui_transition_mappings_count;
    UiPanelId ui_panel_id;
} UiState;

UiPanelId UiState_GetUiPanelIdFromUiActionId(UiState* ui_state, UiActionId ui_action_id);

#endif
