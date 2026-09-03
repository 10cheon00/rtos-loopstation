#include "system_state_map.hpp"

#include "enum_map.hpp"
#include "system_state_error.hpp"
#include "system_state_not_inited.hpp"
#include "system_state_running.hpp"

static constexpr EnumMap<SystemStateMachine::Id, SystemStatePointer>
    system_state_map{
        EnumEntry{SystemStateMachine::Id::NONE, nullptr},
        EnumEntry{SystemStateMachine::Id::NOT_INITED, &SYSTEM_STATE_NOT_INITED},
        EnumEntry{SystemStateMachine::Id::RUNNING, &SYSTEM_STATE_RUNNING},
        EnumEntry{SystemStateMachine::Id::ERROR, &SYSTEM_STATE_ERROR},
    };

SystemStatePointer SystemStateConfigMap_Get(SystemStateMachine::Id id) {
  return system_state_map[id];
}
