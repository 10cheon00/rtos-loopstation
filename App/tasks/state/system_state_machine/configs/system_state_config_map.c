#include "system_state_config_map.h"

#include "system_state_not_inited.h"
#include "system_state_running.h"
#include "system_state_error.h"
#include "config_map.h"
#include "config_validator.h"

static ConfigMapEntry system_state_config_map_entries[] = {
    {.key = SYSTEM_STATE_ID_NOT_INITED, .value = (uint32_t)&SYSTEM_STATE_NOT_INITED},
    {.key = SYSTEM_STATE_ID_RUNNING, .value = (uint32_t)&SYSTEM_STATE_RUNNING},
    {.key = SYSTEM_STATE_ID_ERROR, .value = (uint32_t)&SYSTEM_STATE_ERROR},
};

static ConfigMap system_state_map = {
    .entries = system_state_config_map_entries,
    .count = ARRAY_COUNT(system_state_config_map_entries),
};

ConfigValidator_REGISTER(&system_state_map, SystemStateId, SystemStatePointer);

SystemStatePointer SystemStateConfigMap_Get(SystemStateId id)
{
    Value_t value;
    if (ConfigMap_Get(&system_state_map, id, &value) != CONFIG_MAP_RESULT_OK) {
        return NULL;
    }
    return (SystemStatePointer)value;
}
