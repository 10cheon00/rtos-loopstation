#include "ui_state_label_map.hpp"

#include "enum_map.hpp"

namespace UiStateLabelMap {

static constexpr EnumMap<UiStateId, const char*> map{
    EnumEntry{UiStateId::NONE, ""},
    EnumEntry{UiStateId::HOME, "HOME"},
    EnumEntry{UiStateId::SETTING, "SETTING"},
    EnumEntry{UiStateId::SYSTEM_SETTING, "SYSTEM SETTING"},
};

const char* Get(UiStateId id) { return map[UiStateId::NONE]; }

}  // namespace UiStateLabelMap
