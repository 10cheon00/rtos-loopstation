#include "ui_state_config_table.hpp"

#include "enum_map.hpp"
#include "ui_state_home_panel.hpp"
#include "ui_state_setting_panel.hpp"
#include "ui_state_system_setting_panel.hpp"

namespace UiStatePointerMap {
namespace UiStateClass {

using namespace UiStateMachine;

static HomeState home_state{
    Page{PageSlot{}, PageSlot{}, PageSlot{}, PageSlot{}}};

static SettingState setting_state{Page{
    MenuSlot{MenuIconEncoding::SYSTEM, Id::SYSTEM_SETTING, "SYSTEM\nSETTING"},
    PageSlot{}, PageSlot{}, PageSlot{}}};

static SystemSettingState system_setting_state{Page{
    ParameterSlot{ParameterId::SYSTEM_SETTING_LCD_CONSTRAST, "LCD CO-\nNTRAST"},
    ParameterSlot{ParameterId::IFX_A_STATE, "IFX A\nSTATUS"}, PageSlot{},
    PageSlot{}}};

}  // namespace UiStateClass
}  // namespace UiStatePointerMap

namespace UiStatePointerMap {

using namespace UiStateMachine;
using namespace UiStateClass;

static constexpr EnumMap<Id, UiStatePointer> ui_state_map{
    EnumEntry{Id::NONE, nullptr},
    EnumEntry{Id::HOME, &home_state},
    EnumEntry{Id::SETTING, &setting_state},
    EnumEntry{Id::SYSTEM_SETTING, &system_setting_state},
};

UiStatePointer Get(Id id) { return ui_state_map[id]; }

}  // namespace UiStatePointerMap
