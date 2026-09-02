#include "ui_state_config_table.h"

#include "enum_map.hpp"
#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"
#include "ui_state_system_setting_panel.h"

static constexpr EnumMap<UiStateId, UiStatePointer, UI_STATE_ID_COUNT>
    ui_state_map{
        EnumEntry{UI_STATE_ID_NONE, nullptr},
        EnumEntry{UI_STATE_ID_HOME, &UI_STATE_HOME_PANEL},
        EnumEntry{UI_STATE_ID_SETTING, &UI_STATE_SETTING_PANEL},
        EnumEntry{UI_STATE_ID_SYSTEM_SETTING, &UI_STATE_SYSTEM_SETTING_PANEL},
    };

UiStatePointer UiStateConfigTable_Get(UiStateId id) {
  if (id <= UI_STATE_ID_NONE || id >= UI_STATE_ID_COUNT) {
    return NULL;
  }
  return ui_state_map[id];
}
