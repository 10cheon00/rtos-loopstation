#include "ui_state_system_setting_panel.h"

#include "utils.h"

static PanelSlot panel_slots[4] = {
    {
        .type = PANEL_SLOT_TYPE_PARAMETER,
        .data.parameter =
            (ParameterDescriptor){
                .id = PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST,
                .label = "LCD CO-\nNTRAST",
            },
    },
    {
        .type = PANEL_SLOT_TYPE_PARAMETER,
        .data.parameter =
            (ParameterDescriptor){
                .id = PARAMETER_ID_IFX_A_STATE,
                .label = "IFX A\nSTATUS",
            },
    },
    {
        .type = PANEL_SLOT_TYPE_NONE,
    },
    {
        .type = PANEL_SLOT_TYPE_NONE,
    },
};

UiState UI_STATE_SYSTEM_SETTING_PANEL = {
    .panel_slots = panel_slots,
    .ui_state_id = UI_STATE_ID_SYSTEM_SETTING,
};
