#include "system_state_config_table.h"

#include "enum_map.hpp"
#include "system_state_error.h"
#include "system_state_not_inited.h"
#include "system_state_running.h"

static constexpr EnumMap<SystemStateId, SystemStatePointer,
                         SYSTEM_STATE_ID_COUNT>
    system_state_map{
        EnumEntry{SYSTEM_STATE_ID_NONE, nullptr},
        EnumEntry{SYSTEM_STATE_ID_NOT_INITED, &SYSTEM_STATE_NOT_INITED},
        EnumEntry{SYSTEM_STATE_ID_RUNNING, &SYSTEM_STATE_RUNNING},
        EnumEntry{SYSTEM_STATE_ID_ERROR, &SYSTEM_STATE_ERROR},
    };

SystemStatePointer SystemStateConfigMap_Get(SystemStateId id) {
  if (id <= SYSTEM_STATE_ID_NONE || id >= SYSTEM_STATE_ID_COUNT) {
    return NULL;
  }
  return system_state_map[id];
}
