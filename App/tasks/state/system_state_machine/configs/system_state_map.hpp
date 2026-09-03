#ifndef SYSTEM_STATE_MAP_HPP
#define SYSTEM_STATE_MAP_HPP

#include "system_state.hpp"

typedef SystemStateMachine::State* SystemStatePointer;

SystemStatePointer SystemStateConfigMap_Get(SystemStateMachine::Id id);

#endif
