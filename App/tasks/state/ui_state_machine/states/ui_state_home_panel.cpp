#include "ui_state_home_panel.h"

#include "utils.h"

static PanelSlot panel_slots[4] = {
    {
        .type = PANEL_SLOT_TYPE_NONE,
    },
    {
        .type = PANEL_SLOT_TYPE_NONE,
    },
    {
        .type = PANEL_SLOT_TYPE_NONE,
    },
    {
        .type = PANEL_SLOT_TYPE_NONE,
    },
};

UiState UI_STATE_HOME_PANEL = {
    .ui_state_id = UI_STATE_ID_HOME,
    .page_index = 0,
    .panel_slot_count = 4,
    .panel_slots = panel_slots,
};
