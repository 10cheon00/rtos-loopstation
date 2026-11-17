#include "ui_state_label_map.hpp"

#include "enum_map.hpp"

namespace UiStateLabelMap {

using namespace UiStateMachine;

static constexpr EnumMap<Id, const char*> map{
    {Id::NONE, ""},
    {Id::HOME, "HOME"},
    {Id::SETTING, "SETTING"},
    {Id::SYSTEM_SETTING, "SYSTEM SETTING"},
};

const char* Get(Id id) { return map[id]; }

}  // namespace UiStateLabelMap
