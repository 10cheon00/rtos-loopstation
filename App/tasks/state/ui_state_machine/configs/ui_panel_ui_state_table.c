#include "ui_panel_ui_state_table.h"

#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"
#include "ui_state_system_setting_panel.h"

UiState *ui_panel_ui_state_table[UI_PANEL_ID_COUNT] = {
    [UI_PANEL_ID_NONE] = 0,
    [UI_PANEL_ID_HOME] = &UI_STATE_HOME_PANEL,
    [UI_PANEL_ID_SETTING] = &UI_STATE_SETTING_PANEL,
    [UI_PANEL_ID_SYSTEM_SETTING] = &UI_STATE_SYSTEM_SETTING_PANEL};

UiState *UiPanelUiStateTable_GetUiStateFromUiPanelId(UiPanelId ui_panel_id)
{
    if (ui_panel_id <= UI_PANEL_ID_NONE && ui_panel_id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return ui_panel_ui_state_table[ui_panel_id];
}
