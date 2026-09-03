#ifndef UI_STATE_H
#define UI_STATE_H

#include <stdbool.h>
#include <stddef.h>

#include "id.h"
#include "menu_descriptor.hpp"
#include "panel_slot_type.h"
#include "parameter_descriptor.h"
#include "ui_state_id.h"
#include "ui_state_slot_index.h"

/* 순환 참조를 막기 위한 전방 선언 */
typedef struct UiStateMachine UiStateMachine;

typedef struct {
  PanelSlotType type;
  union Data {
    MenuDescriptor menu;
    ParameterDescriptor parameter;
  } data;
} PanelSlot;

typedef PanelSlot* (*UiStatePanelSlotGetterFunction)(UiStateSlotIndex id);

typedef struct {
  UiStateId ui_state_id;
  uint8_t page_index;
  uint8_t panel_slot_count;
  PanelSlot* panel_slots;
} UiState;

PanelSlot* UiState_GetPanelSlot(UiState* ui_state, UiStateSlotIndex id);
void UiState_IncreasePageIndex(UiState* ui_state);
bool UiState_CanIncreasePageIndex(UiState* ui_state);
void UiState_DecreasePageIndex(UiState* ui_state);
bool UiState_CanDecreasePageIndex(UiState* ui_state);
#endif
