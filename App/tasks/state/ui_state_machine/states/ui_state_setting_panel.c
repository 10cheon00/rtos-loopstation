#include "ui_state_setting_panel.h"

#include "utils.h"

static PanelSlot panel_slots[4] = {
    {
        .type = PANEL_SLOT_TYPE_MENU,
        .data.menu =
            {
                .icon_id = MENU_ICON_ID_SYSTEM,
                .state_id = UI_STATE_ID_SYSTEM_SETTING,
                .label = "SYSTEM\nSETTING"
            },
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

UiState UI_STATE_SETTING_PANEL = {
    .panel_slots = panel_slots,
    .ui_state_id = UI_STATE_ID_SETTING,
};
