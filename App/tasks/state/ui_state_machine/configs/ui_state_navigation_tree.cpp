#include "ui_state_navigation_tree.h"

#include "enum_map.hpp"

static constexpr EnumMap<UiStateId, UiStateId> parent{
    EnumEntry{UiStateId::NONE, UiStateId::NONE},
    EnumEntry{UiStateId::HOME, UiStateId::NONE},
    EnumEntry{UiStateId::SETTING, UiStateId::HOME},
    EnumEntry{UiStateId::SYSTEM_SETTING, UiStateId::SETTING},
};

UiStateId UiStateNavigationTree_GetParent(UiStateId id) { return parent[id]; }
