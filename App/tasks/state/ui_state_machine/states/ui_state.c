#include "ui_state.h"

PanelSlot *UiState_GetPanelSlot(UiState *ui_state, UiStateSlotIndex id)
{
    uint8_t index;
    if (id < UI_STATE_SLOT_INDEX_A || id >= UI_STATE_SLOT_INDEX_COUNT) {
        return NULL;
    }
    index = ui_state->page_index * UI_STATE_SLOT_INDEX_COUNT + (uint8_t)id;
    if (index < 0 || index >= ui_state->panel_slot_count) {
        return NULL;
    }
    return &ui_state->panel_slots[index];
}

void UiState_IncreasePageIndex(UiState *ui_state)
{
    if ((ui_state->page_index + 1) * UI_STATE_SLOT_INDEX_COUNT <= ui_state->panel_slot_count) {
        ui_state->page_index++;
    }
}

void UiState_DecreasePageIndex(UiState *ui_state)
{
    if (ui_state->page_index > 0) {
        ui_state->page_index--;
    }
}
