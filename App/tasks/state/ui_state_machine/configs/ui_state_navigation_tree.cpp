#include "ui_state_navigation_tree.h"

#include <array>

static constexpr auto parent = [] {
  std::array<UiStateId, UI_STATE_ID_COUNT> values{};
  values[UI_STATE_ID_NONE] = UI_STATE_ID_NONE;
  values[UI_STATE_ID_HOME] = UI_STATE_ID_NONE;
  values[UI_STATE_ID_SETTING] = UI_STATE_ID_HOME;
  values[UI_STATE_ID_SYSTEM_SETTING] = UI_STATE_ID_SETTING;
  return values;
}();

UiStateId UiStateNavigationTree_GetParent(UiStateId id) {
  if (id < UI_STATE_ID_NONE || id >= UI_STATE_ID_COUNT) {
    return UI_STATE_ID_NONE;
  }
  return parent[id];
}
