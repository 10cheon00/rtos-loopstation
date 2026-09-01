#include "ui_state_label_config_table.h"

#include <array>

static constexpr auto table = [] {
    std::array<const char *, UI_STATE_ID_COUNT> values{};
    values[UI_STATE_ID_NONE] = "";
    values[UI_STATE_ID_HOME] = "HOME";
    values[UI_STATE_ID_SETTING] = "SETTING";
    values[UI_STATE_ID_SYSTEM_SETTING] = "SYSTEM SETTING";
    return values;
}();

const char *UiStateLabelConfigTable_Get(UiStateId id)
{
    if (id < UI_STATE_ID_NONE || id >= UI_STATE_ID_COUNT) {
        return table[UI_STATE_ID_NONE];
    }
    return table[id];
}
