#include "ui_state_system_setting_panel.h"

#include "utils.h"

static UiTransitionMapEntry transition_map[] = {
    {.ui_action_id = UI_ACTION_ID_EXIT, .next_ui_state_id = UI_STATE_ID_SETTING},
};
static ParameterSlotConfig slots[4] = {
    {
        .id = PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST,
        .label = "LCD CO-\nNTRAST",
    },
    {.id = PARAMETER_ID_IFX_A_STATE, .label = "IFX A"},
};

static ParameterSlotConfig slots2[4] = {
    {.id = PARAMETER_ID_IFX_A_STATE, .label = "IFX A"},
    {
        .id = PARAMETER_ID_SYSTEM_SETTING_LCD_CONSTRAST,
        .label = "LCD CO-\nNTRAST",
    },
};
static ParameterSlotConfig *slot_ptr = slots;

static ParameterSlotConfig *ParameterSlotGetter();

static void OnUiActionEvent(UiActionId id);

UiState UI_STATE_SYSTEM_SETTING_PANEL = {
    .ui_transition_map = transition_map,
    .ui_transition_map_count = ARRAY_COUNT(transition_map),
    .parameter_slot_getter_function = ParameterSlotGetter,
    .OnUiActionEvent = OnUiActionEvent,
    .ui_state_id = UI_STATE_ID_SYSTEM_SETTING};

ParameterSlotConfig *ParameterSlotGetter()
{
    return slot_ptr;
}

static void OnUiActionEvent(UiActionId id) {
    if (id == UI_ACTION_ID_NAVIGATE_LEFT) {
        slot_ptr = slots2;
    } else if (id == UI_ACTION_ID_NAVIGATE_RIGHT) {
        slot_ptr = slots;
    }
}
