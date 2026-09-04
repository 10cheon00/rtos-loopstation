#include "system_state_pointer_map.hpp"

#include "enum_map.hpp"

namespace SystemStatePointerMap {

using namespace SystemStateClass;

static NotInitState not_init_state;
static RunningState running_state;
static ErrorState error_state;

static constexpr EnumMap<SystemStateMachine::Id, SystemStatePointer>
    system_state_map{
        EnumEntry{SystemStateMachine::Id::NONE, nullptr},
        EnumEntry{SystemStateMachine::Id::NOT_INITED, &not_init_state},
        EnumEntry{SystemStateMachine::Id::RUNNING, &running_state},
        EnumEntry{SystemStateMachine::Id::ERROR, &error_state},
    };

SystemStatePointer Get(SystemStateMachine::Id id) {
  return system_state_map[id];
}

}  // namespace SystemStatePointerMap
