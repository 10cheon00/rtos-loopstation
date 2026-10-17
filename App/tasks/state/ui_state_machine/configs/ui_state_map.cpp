#include "ui_state_map.hpp"

#include "enum_map.hpp"
#include "ui_state.hpp"

namespace UiStateMap {

using namespace UiStateMachine;

namespace UiStatePageGroup {

static NonePageGroup none_page_group;
static HomePageGroup home_page_group;

}  // namespace UiStatePageGroup

static NoneState none_state{UiStatePageGroup::none_page_group};
static HomeState home_state{UiStatePageGroup::home_page_group};
// static constexpr State setting_state{Id::SETTING, "SETTING"};
// static constexpr State system_setting_state{Id::SYSTEM_SETTING,
//                                             "SYSTEM SETTING"};
// static constexpr State ifx_a_edit_state{Id::IFX_A_EDIT, "IFX n EDIT"};

static EnumMap<Id, StatePointer> ui_state_map{
    EnumEntry{Id::NONE, &none_state},
    EnumEntry{Id::HOME, &home_state},
    // EnumEntry{Id::SETTING, setting_state},
    // EnumEntry{Id::SYSTEM_SETTING, system_setting_state},
    // EnumEntry{Id::IFX_A_EDIT, ifx_a_edit_state},
};

StatePointer Get(Id id) { return ui_state_map[id]; }

}  // namespace UiStateMap
