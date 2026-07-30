#include "ui_panel_id_ui_state_mapping.h"

#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"

UiState *ui_panel_id_ui_state_mapping[UI_PANEL_ID_COUNT] = {
    [UI_PANEL_ID_NONE] = 0,
    [UI_PANEL_ID_HOME] = &UI_STATE_HOME_PANEL,
    [UI_PANEL_ID_SETTING] = &UI_STATE_SETTING_PANEL};

UiState *UiPanelIdUiStateMapping_GetUiStateFromUiPanelId(UiPanelId ui_panel_id)
{
    if (ui_panel_id <= UI_PANEL_ID_NONE && ui_panel_id >= UI_PANEL_ID_COUNT) {
        return NULL;
    }
    return ui_panel_id_ui_state_mapping[ui_panel_id];
}
