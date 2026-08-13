#include "system_state_config_table.h"

#include "system_state_not_inited.h"
#include "system_state_running.h"
#include "system_state_error.h"
#include "config_table.h"
#include "config_validator.h"

#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(SYSTEM_STATE_ID_NOT_INITED, (Value_t) & SYSTEM_STATE_NOT_INITED),         \
        ConfigTable_1D_ENTRY(SYSTEM_STATE_ID_RUNNING, (Value_t) & SYSTEM_STATE_RUNNING),           \
        ConfigTable_1D_ENTRY(SYSTEM_STATE_ID_ERROR, (Value_t) & SYSTEM_STATE_ERROR),

ConfigTable_1D_DECLARE_TABLE(SystemStateId, SystemStatePointer, SYSTEM_STATE_ID_COUNT, ENTRIES);
#undef ENTRIES

ConfigValidator_REGISTER_CONFIG_TABLE_1D(SystemStateId, SystemStatePointer, SYSTEM_STATE_ID_COUNT,
                                         0, UINT32_MAX, CONFIG_TABLE_TYPE_NO_NULL_VALUE);

SystemStatePointer SystemStateConfigMap_Get(SystemStateId id)
{
    if (id <= SYSTEM_STATE_ID_NONE || id >= SYSTEM_STATE_ID_COUNT) {
        return NULL;
    }
    return (SystemStatePointer)ConfigTable_1D_GET(SystemStateId, SystemStatePointer, id);
}
