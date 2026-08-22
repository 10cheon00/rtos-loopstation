#include "ui_state_system_setting_panel.h"

#include "utils.h"

UiTransitionMapEntry ui_system_setting_panel_transition_map[] = {
    {.ui_action_id = UI_ACTION_ID_EXIT, .next_ui_state_id = UI_STATE_ID_SETTING},
};
static ParameterSlotConfig slots[4] = {
    {
        .id = PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST,
        .label = "LCD CO-\nNTRAST",
    },
    {.id = PARAMETER_ID_IFX_A_STATE, .label = "IFX A"},
};
static ParameterSlotConfig *ParameterSlotGetter();

UiState UI_STATE_SYSTEM_SETTING_PANEL = {
    .ui_transition_map = ui_system_setting_panel_transition_map,
    .ui_transition_map_count = ARRAY_COUNT(ui_system_setting_panel_transition_map),
    .parameter_slot_getter_function = ParameterSlotGetter,
    .ui_state_id = UI_STATE_ID_SYSTEM_SETTING};

ParameterSlotConfig *ParameterSlotGetter()
{
    return slots;
}
