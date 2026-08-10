#include "config_map.h"

// TODO:
// 탐색 기능이 O(n)의 시간복잡도를 갖는데, 더 단축할 수 없을까?

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

ConfigMapResult ConfigMap_Contains(ConfigMap *map, Key_t key)
{
    for (ConfigMapCount_t i = 0; i < map->count; i++) {
        if (map->entries[i].key == key) {
            return CONFIG_MAP_RESULT_OK;
        }
    }
    return CONFIG_MAP_RESULT_ERROR;
}
