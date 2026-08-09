#include "config_map.h"

ConfigMapResult ConfigMap_Get(ConfigMap *map, Key_t key, Value_t *value)
{
    for (ConfigMapCount_t i = 0; i < map->count; i++) {
        if (map->entries[i].key == key) {
            *value = map->entries[i].value;
            return CONFIG_MAP_RESULT_OK;
        }
    }
    return CONFIG_MAP_RESULT_ERROR;
}
