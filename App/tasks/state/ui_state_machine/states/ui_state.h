#ifndef UI_STATE_H
#define UI_STATE_H

#include <stddef.h>

#include "ui_panel_id.h"

// TODO:
// UiStateEvent는 더 큰 범주를 의미하는 것 같다. 이벤트가 전이 이벤트 하나뿐인가?
typedef enum {
    UI_STATE_EVENT_NONE,
    UI_STATE_EVENT_NAVIGATE_LEFT,
    UI_STATE_EVENT_NAVIGATE_RIGHT,
    UI_STATE_EVENT_ENTER,
    UI_STATE_EVENT_EXIT,
    UI_STATE_EVENT_TO_IFX,
    UI_STATE_EVENT_TO_TFX,
    UI_STATE_EVENT_COUNT
} UiStateEventId;

/* 순환 참조를 막기 위한 전방 선언 */
typedef struct UiStateMachine UiStateMachine;

typedef struct {
    UiStateEventId ui_state_event_id;
    UiPanelId next_ui_panel_id;
} UiTransitionMapping;

typedef struct {
    UiTransitionMapping* ui_transition_mappings;
    size_t ui_transition_mappings_count;
    UiPanelId ui_panel_id;
} UiState;

UiPanelId UiState_GetUiPanelIdFromUiStateEventId(UiState* ui_state, UiStateEventId ui_state_event_id);

#endif
