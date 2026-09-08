#include "ui_state_label_map.hpp"

#include "enum_map.hpp"

namespace UiStateLabelMap {

using namespace UiStateMachine;

static constexpr EnumMap<Id, const char*> map{
    EnumEntry{Id::NONE, ""},
    EnumEntry{Id::HOME, "HOME"},
    EnumEntry{Id::SETTING, "SETTING"},
    EnumEntry{Id::SYSTEM_SETTING, "SYSTEM SETTING"},
};

const char* Get(Id id) { return map[Id::NONE]; }

}  // namespace UiStateLabelMap
