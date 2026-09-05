#include "ui_state_config_table.hpp"

#include "enum_map.hpp"
#include "ui_state_home_panel.hpp"
#include "ui_state_setting_panel.hpp"
#include "ui_state_system_setting_panel.hpp"

namespace UiStatePointerMap {
namespace UiStateClass {

using namespace UiStateMachine;

static HomeState home_state{Page{(PanelSlot){
                                     .type = PanelSlotType::NONE,
                                 },
                                 (PanelSlot){
                                     .type = PanelSlotType::NONE,
                                 },
                                 (PanelSlot){
                                     .type = PanelSlotType::NONE,
                                 },
                                 (PanelSlot){
                                     .type = PanelSlotType::NONE,
                                 }}};
static SettingState setting_state{Page{
    (PanelSlot){.type = PanelSlotType::MENU,
                .data =
                    {
                        .menu = (MenuDescriptor){.icon_encoding = MenuIconEncoding::SYSTEM,
                                                 .state_id = Id::SYSTEM_SETTING,
                                                 .label = "SYSTEM\nSETTING"},
                    }},
    (PanelSlot){
        .type = PanelSlotType::NONE,
    },
    (PanelSlot){
        .type = PanelSlotType::NONE,
    },
    (PanelSlot){
        .type = PanelSlotType::NONE,
    }}};
static SystemSettingState system_setting_state{Page{
    (PanelSlot){.type = PanelSlotType::PARAMETER,
                .data =
                    {
                        .parameter =
                            (ParameterDescriptor){
                                .id = ParameterId::SYSTEM_SETTING_LCD_CONSTRAST,
                                .label = "LCD CO-\nNTRAST",
                            },
                    }},
    (PanelSlot){.type = PanelSlotType::PARAMETER,
                .data =
                    {
                        .parameter =
                            (ParameterDescriptor){
                                .id = ParameterId::IFX_A_STATE,
                                .label = "IFX A\nSTATUS",
                            },
                    }},
    (PanelSlot){
        .type = PanelSlotType::NONE,
    },
    (PanelSlot){
        .type = PanelSlotType::NONE,
    }}};

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
