#include "ui_state_ui_panel_id_mapping.h"

#include "utils.h"

const State* ui_state_ui_panel_id_mappings[UI_PANEL_ID_COUNT] = {
    [UI_PANEL_ID_NONE] = 0,
    [UI_PANEL_ID_HOME] = &UI_STATE_HOME_PANEL,
    [UI_PANEL_ID_SETTING] = &UI_STATE_SETTING_PANEL,
};

const size_t UI_STATE_UI_PANEL_ID_MAPPING_COUNT = ARRAY_COUNT(ui_state_ui_panel_id_mappings);
