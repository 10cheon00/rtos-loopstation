#ifndef CONFIG_TABLE_H
#define CONFIG_TABLE_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "utils.h"

typedef uint16_t Key_t;
typedef uint32_t Value_t;

template <typename IndexType>
constexpr std::size_t to_index(IndexType value) noexcept {
  return static_cast<std::size_t>(value);
}

template <typename ConfigValue>
Value_t ConfigTable_ToValue(ConfigValue value) {
  if constexpr (std::is_pointer_v<ConfigValue>) {
    return static_cast<Value_t>(reinterpret_cast<std::uintptr_t>(value));
  } else {
    return static_cast<Value_t>(value);
  }
}

#define ConfigTable_NAME(KEY_TYPE, VALUE_TYPE) \
  CONCATENATE2(KEY_TYPE, CONCATENATE2(VALUE_TYPE, _config_table))

#define ConfigTable_1D_ENTRY(KEY, VALUE) values[to_index(KEY)] = VALUE
#define ConfigTable_2D_ENTRY(KEY, ...) values[to_index(KEY)] = {__VA_ARGS__}

#define ConfigTable_1D_DECLARE_TABLE(KEY_TYPE, VALUE_TYPE, COUNT, ENTRIES) \
  static constexpr auto ConfigTable_NAME(KEY_TYPE, VALUE_TYPE) = [] {      \
    std::array<VALUE_TYPE, COUNT> values{};                                \
    ENTRIES;                                                               \
    return values;                                                         \
  }()
#define ConfigTable_2D_DECLARE_TABLE(KEY_TYPE, VALUE_TYPE, ROW_COUNT,     \
                                     COLUMN_COUNT, ENTRIES)               \
  static constexpr auto ConfigTable_NAME(KEY_TYPE, VALUE_TYPE) = [] {     \
    std::array<std::array<VALUE_TYPE, COLUMN_COUNT>, ROW_COUNT> values{}; \
    ENTRIES;                                                              \
    return values;                                                        \
  }()
#define ConfigTable_1D_GET(KEY_TYPE, VALUE_TYPE, INDEX) \
  ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[INDEX]
#define ConfigTable_2D_GET_ROW(KEY_TYPE, VALUE_TYPE, ROW_INDEX) \
  ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[ROW_INDEX]
#define ConfigTable_2D_GET(KEY_TYPE, VALUE_TYPE, ROW_INDEX, COLUMN_INDEX) \
  ConfigTable_NAME(KEY_TYPE, VALUE_TYPE)[ROW_INDEX][COLUMN_INDEX]
#endif
