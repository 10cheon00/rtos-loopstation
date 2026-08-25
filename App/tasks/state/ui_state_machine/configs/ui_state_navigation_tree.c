#include "ui_state_navigation_tree.h"

static UiStateId parent[UI_STATE_ID_COUNT] = {
    [UI_STATE_ID_NONE] = UI_STATE_ID_NONE,
    [UI_STATE_ID_HOME] = UI_STATE_ID_NONE,
    [UI_STATE_ID_SETTING] = UI_STATE_ID_HOME,
    [UI_STATE_ID_SYSTEM_SETTING] = UI_STATE_ID_SETTING,
};

UiStateId UiStateNavigationTree_GetParent(UiStateId id)
{
    if (id < UI_STATE_ID_NONE || id >= UI_STATE_ID_COUNT) {
        return UI_STATE_ID_NONE;
    }
    return parent[id];
}
