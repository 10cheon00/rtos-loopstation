#include "ui_state.h"

PanelSlot *UiState_GetPanelSlot(UiState *ui_state, UiStateSlotIndex id)
{
    if (id < UI_STATE_SLOT_INDEX_A || id >= UI_STATE_SLOT_INDEX_COUNT) {
        return NULL;
    }
    return &ui_state->panel_slots[ui_state->page_index * 4 + (uint8_t)id];
}
