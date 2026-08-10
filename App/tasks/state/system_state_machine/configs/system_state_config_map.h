#ifndef SYSTEM_STATE_CONFIG_MAP_H
#define SYSTEM_STATE_CONFIG_MAP_H

#include "system_state.h"

typedef SystemState *SystemStatePointer;

SystemStatePointer SystemStateConfigMap_Get(SystemStateId id);

#endif
