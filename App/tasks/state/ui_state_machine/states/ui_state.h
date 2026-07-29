#ifndef UI_STATE_H
#define UI_STATE_H

#include "ui_panel_id.h"

typedef enum {
    UI_STATE_EVENT_NAVIGATE_LEFT,
    UI_STATE_EVENT_NAVIGATE_RIGHT,
    UI_STATE_EVENT_ENTER,
    UI_STATE_EVENT_EXIT,
} UiStateEventId;

/* 순환 참조를 막기 위한 전방 선언 */
typedef struct UiStateMachine UiStateMachine;

typedef void (*UiState_OnEnterFunction)(UiStateMachine *ui_state_machine);
typedef void (*UiState_OnEventFunction)(UiStateMachine *ui_state_machine,
                                        UiStateEventId ui_state_event_id);

typedef struct {
    UiState_OnEnterFunction UiState_OnEnter;
    UiState_OnEventFunction UiState_OnEvent;
    UiPanelId ui_panel_id;
} UiState;

#endif
