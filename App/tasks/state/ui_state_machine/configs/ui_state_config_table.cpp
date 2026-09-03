#include "ui_state_config_table.h"

#include "enum_map.hpp"
#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"
#include "ui_state_system_setting_panel.h"

static constexpr EnumMap<UiStateId, UiStatePointer> ui_state_map{
    EnumEntry{UiStateId::NONE, nullptr},
    EnumEntry{UiStateId::HOME, &UI_STATE_HOME_PANEL},
    EnumEntry{UiStateId::SETTING, &UI_STATE_SETTING_PANEL},
    EnumEntry{UiStateId::SYSTEM_SETTING, &UI_STATE_SYSTEM_SETTING_PANEL},
};

UiStatePointer UiStateConfigTable_Get(UiStateId id) {
  if (id <= UiStateId::NONE || id >= UiStateId::COUNT) {
    return NULL;
  }
  return ui_state_map[id];
}
