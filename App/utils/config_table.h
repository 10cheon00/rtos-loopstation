#ifndef CONFIG_TABLE_H
#define CONFIG_TABLE_H

#define CONCATENATE2_IMPL(A, B) A##B
#define CONCATENATE2(A, B) CONCATENATE2_IMPL(A, B)
#define ConfigTable_NAME(KEY_TYPE, VALUE_TYPE) CONCATENATE2(KEY_TYPE, VALUE_TYPE)

#define ConfigTable_1D_ENTRY(KEY, VALUE) [KEY] = VALUE
#define ConfigTable_2D_ENTRY(KEY, ...) [KEY] = {__VA_ARGS__}

#define ConfigTable_1D_DECLARE_TABLE(KEY_TYPE, VALUE_TYPE, COUNT, ENTRIES)                         \
    static VALUE_TYPE ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[COUNT] = {ENTRIES}
#define ConfigTable_2D_DECLARE_TABLE(KEY_TYPE, VALUE_TYPE, ROW_COUNT, COLUMN_COUNT, ENTRIES)       \
    static VALUE_TYPE ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[ROW_COUNT][COLUMN_COUNT] = {ENTRIES}
#define ConfigTable_1D_GET(KEY_TYPE, VALUE_TYPE, INDEX)                                            \
    ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[INDEX]
#define ConfigTable_2D_GET_ROW(KEY_TYPE, VALUE_TYPE, ROW_INDEX)                                    \
    ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[ROW_INDEX]
#define ConfigTable_2D_GET(KEY_TYPE, VALUE_TYPE, ROW_INDEX, COLUMN_INDEX)                          \
    ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[ROW_INDEX][COLUMN_INDEX]
#endif
