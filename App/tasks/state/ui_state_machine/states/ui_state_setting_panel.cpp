#include "ui_state_setting_panel.h"

#include "utils.h"

static PanelSlot panel_slots[4] = {
    {.type = PANEL_SLOT_TYPE_MENU,
     .data =
         {
             .menu = (MenuDescriptor){.icon_id = MENU_ICON_ID_SYSTEM,
                                      .state_id = UiStateId::SYSTEM_SETTING,
                                      .label = "SYSTEM\nSETTING"},
         }},
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
    .ui_state_id = UiStateId::SETTING,
    .page_index = 0,
    .panel_slot_count = ARRAY_COUNT(panel_slots),
    .panel_slots = panel_slots,
};
