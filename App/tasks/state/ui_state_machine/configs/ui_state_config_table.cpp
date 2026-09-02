#include "ui_state_config_table.h"

#include "config_table.h"
#include "config_validator.h"
#include "ui_state_home_panel.h"
#include "ui_state_setting_panel.h"
#include "ui_state_system_setting_panel.h"

#define ENTRIES                                                           \
  ConfigTable_1D_ENTRY(UI_STATE_ID_NONE, nullptr),                        \
      ConfigTable_1D_ENTRY(UI_STATE_ID_HOME, &UI_STATE_HOME_PANEL),       \
      ConfigTable_1D_ENTRY(UI_STATE_ID_SETTING, &UI_STATE_SETTING_PANEL), \
      ConfigTable_1D_ENTRY(UI_STATE_ID_SYSTEM_SETTING,                    \
                           &UI_STATE_SYSTEM_SETTING_PANEL)

ConfigTable_1D_DECLARE_TABLE(UiStateId, UiStatePointer, UI_STATE_ID_COUNT,
                             ENTRIES);

ConfigValidator_REGISTER_CONFIG_TABLE_1D(UiStateId, UiStatePointer,
                                         UI_STATE_ID_COUNT, 0, UINT32_MAX,
                                         CONFIG_TABLE_TYPE_NO_NULL_VALUE);

UiStatePointer UiStateConfigTable_Get(UiStateId id) {
  if (id <= UI_STATE_ID_NONE || id >= UI_STATE_ID_COUNT) {
    return NULL;
  }
  return ConfigTable_1D_GET(UiStateId, UiStatePointer, id);
}
