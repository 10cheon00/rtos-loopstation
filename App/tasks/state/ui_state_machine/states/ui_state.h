#ifndef UI_STATE_H
#define UI_STATE_H

#include <stddef.h>

#include "id.h"
#include "ui_panel_id.h"
#include "parameter_slot.h"

typedef enum {
    UI_ACTION_ID_NONE = ID_NONE,
    UI_ACTION_ID_NULL = ID_NULL,
    UI_ACTION_ID_NAVIGATE_LEFT,
    UI_ACTION_ID_NAVIGATE_RIGHT,
    UI_ACTION_ID_ENTER,
    UI_ACTION_ID_ENTER_ENCODER_A,
    UI_ACTION_ID_ENTER_ENCODER_B,
    UI_ACTION_ID_ENTER_ENCODER_C,
    UI_ACTION_ID_ENTER_ENCODER_D,
    UI_ACTION_ID_EXIT,
    UI_ACTION_ID_TO_IFX,
    UI_ACTION_ID_TO_TFX,
    UI_ACTION_ID_COUNT
} UiActionId;

/* 순환 참조를 막기 위한 전방 선언 */
typedef struct UiStateMachine UiStateMachine;

typedef struct {
    UiActionId ui_action_id;
    UiPanelId next_ui_panel_id;
} UiTransitionMapEntry;

typedef ParameterSlotConfig* (*ParameterSlotConfigGetterFunction)();

typedef struct {
    UiTransitionMapEntry* ui_transition_map;
    ParameterSlotConfigGetterFunction parameter_slot_getter_function;
    size_t ui_transition_map_count;
    UiPanelId ui_panel_id;
} UiState;

UiPanelId UiState_GetUiPanelIdFromUiActionId(UiState* ui_state, UiActionId ui_action_id);

ParameterSlotConfig* UiState_GetParameterSlots(UiState* ui_state);

#endif
