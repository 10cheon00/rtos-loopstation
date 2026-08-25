#ifndef UI_STATE_H
#define UI_STATE_H

#include <stddef.h>
#include <stdbool.h>

#include "id.h"
#include "ui_state_id.h"
#include "parameter_descriptor.h"
#include "menu_descriptor.h"
#include "ui_state_slot_index.h"

/* 순환 참조를 막기 위한 전방 선언 */
typedef struct UiStateMachine UiStateMachine;

typedef enum {
    PANEL_SLOT_TYPE_NONE = 0,
    PANEL_SLOT_TYPE_MENU,
    PANEL_SLOT_TYPE_PARAMETER,
} PanelSlotType;

typedef struct {
    PanelSlotType type;
    union {
        MenuDescriptor menu;
        ParameterDescriptor parameter;
    } data;
} PanelSlot;

typedef PanelSlot *(*UiStatePanelSlotGetterFunction)(UiStateSlotIndex id);

typedef struct {
    PanelSlot *panel_slots;
    UiStateId ui_state_id;
    uint8_t panel_slot_count;
    uint8_t page_index;
} UiState;

PanelSlot *UiState_GetPanelSlot(UiState *ui_state, UiStateSlotIndex id);
void UiState_IncreasePageIndex(UiState *ui_state);
bool UiState_CanIncreasePageIndex(UiState *ui_state);
void UiState_DecreasePageIndex(UiState *ui_state);
bool UiState_CanDecreasePageIndex(UiState *ui_state);
#endif
