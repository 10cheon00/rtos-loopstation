#include "ui_state_map.hpp"

#include <cstdio>

#include "enum_map.hpp"
#include "fx_page_group_map.hpp"
#include "ui_state.hpp"

namespace UiStateMap {

using namespace UiStateMachine;

namespace UiStatePageGroup {

static NonePageGroup none_page_group;
static HomePageGroup home_page_group;
static SettingPageGroup setting_page_group;
static SystemSettingPageGroup system_setting_page_group;

}  // namespace UiStatePageGroup

static NoneState none_state{UiStatePageGroup::none_page_group};
static HomeState home_state{UiStatePageGroup::home_page_group};
static SettingState setting_state{UiStatePageGroup::setting_page_group};
static SystemSettingState system_setting_state{
    UiStatePageGroup::system_setting_page_group};
static FxEditState fx_edit_state{FxPageGroupMap::Get(Fx::Type::LPF)};

static EnumMap<Id, StatePointer> ui_state_map{
    EnumEntry{Id::NONE, &none_state},
    EnumEntry{Id::HOME, &home_state},
    EnumEntry{Id::SETTING, &setting_state},
    EnumEntry{Id::SYSTEM_SETTING, &system_setting_state},
    EnumEntry{Id::FX_EDIT, &fx_edit_state},
};

StatePointer Get(Id id) { return ui_state_map[id]; }

}  // namespace UiStateMap
