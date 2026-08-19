#ifndef CONFIG_TABLE_H
#define CONFIG_TABLE_H

#include "utils.h"

typedef uint16_t Key_t;
typedef uint32_t Value_t;

#define ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)                                                     \
    CONCATENATE2(KEY_TYPE, CONCATENATE2(VALUE_TYPE, _config_table))

#define ConfigTable_1D_ENTRY(KEY, VALUE) [KEY] = VALUE
#define ConfigTable_2D_ENTRY(KEY, ...) [KEY] = {__VA_ARGS__}

#define ConfigTable_1D_DECLARE_TABLE(KEY_TYPE, VALUE_TYPE, COUNT, ENTRIES)                         \
    static Value_t ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[COUNT] = {ENTRIES}
#define ConfigTable_2D_DECLARE_TABLE(KEY_TYPE, VALUE_TYPE, ROW_COUNT, COLUMN_COUNT, ENTRIES)       \
    static Value_t ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[ROW_COUNT][COLUMN_COUNT] = {ENTRIES}
#define ConfigTable_1D_GET(KEY_TYPE, VALUE_TYPE, INDEX)                                            \
    ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[INDEX]
#define ConfigTable_2D_GET_ROW(KEY_TYPE, VALUE_TYPE, ROW_INDEX)                                    \
    ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[ROW_INDEX]
#define ConfigTable_2D_GET(KEY_TYPE, VALUE_TYPE, ROW_INDEX, COLUMN_INDEX)                          \
    ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[ROW_INDEX][COLUMN_INDEX]
#endif
