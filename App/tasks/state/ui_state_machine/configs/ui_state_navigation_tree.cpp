#include "ui_state_navigation_tree.hpp"

#include "enum_map.hpp"

static constexpr EnumMap<UiStateMachine::Id, UiStateMachine::Id> parent{
    {UiStateMachine::Id::NONE, UiStateMachine::Id::NONE},
    {UiStateMachine::Id::HOME, UiStateMachine::Id::NONE},
    {UiStateMachine::Id::SETTING, UiStateMachine::Id::HOME},
    {UiStateMachine::Id::SYSTEM_SETTING, UiStateMachine::Id::SETTING},
    {UiStateMachine::Id::FX_EDIT, UiStateMachine::Id::HOME},
};

UiStateMachine::Id UiStateNavigationTree_GetParent(UiStateMachine::Id id) {
  return parent[id];
}
