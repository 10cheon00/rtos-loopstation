#include "ui_state_navigation_tree.hpp"

#include "enum_map.hpp"

static constexpr EnumMap<UiStateMachine::Id, UiStateMachine::Id> parent{
    EnumEntry{UiStateMachine::Id::NONE, UiStateMachine::Id::NONE},
    EnumEntry{UiStateMachine::Id::HOME, UiStateMachine::Id::NONE},
    EnumEntry{UiStateMachine::Id::SETTING, UiStateMachine::Id::HOME},
    EnumEntry{UiStateMachine::Id::SYSTEM_SETTING, UiStateMachine::Id::SETTING},
};

UiStateMachine::Id UiStateNavigationTree_GetParent(UiStateMachine::Id id) {
  return parent[id];
}
