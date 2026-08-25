#include "ui_state_label_config_table.h"

static const char *table[UI_STATE_ID_COUNT] = {
    [UI_STATE_ID_NONE] = "",
    [UI_STATE_ID_HOME] = "HOME",
    [UI_STATE_ID_SETTING] = "SETTING",
    [UI_STATE_ID_SYSTEM_SETTING] = "SYSTEM SETTING",
};

const char *UiStateLabelConfigTable_Get(UiStateId id)
{
    if (id < UI_STATE_ID_NONE || id >= UI_STATE_ID_COUNT) {
        return table[UI_STATE_ID_NONE];
    }
    return table[id];
}
