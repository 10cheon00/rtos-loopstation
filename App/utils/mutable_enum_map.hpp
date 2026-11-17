#ifndef MUTABLE_ENUM_MAP_HPP
#define MUTABLE_ENUM_MAP_HPP

#include <array>

#include "enum_map.hpp"

template <typename Key, typename Value,
          std::size_t MapSize = static_cast<std::size_t>(Key::COUNT)>
class MutableEnumMap {
 public:
  constexpr MutableEnumMap() : array{}, assigned{} {}

  /**
   * {key, value} 초기화 목록을 받아 내부 array에 순서대로 대입한다.
   */
  constexpr MutableEnumMap(
      std::initializer_list<EnumEntry<Key, Value>> entries)
      : array{}, assigned{} {
    for (const auto& entry : entries) {
      const auto index = static_cast<std::size_t>(entry.key);
      array[index] = entry.value;
      assigned[index] = true;
    }
  }

  constexpr Value& Get(Key key) { return array[static_cast<std::size_t>(key)]; }

  constexpr Value& operator[](Key key) {
    return array[static_cast<std::size_t>(key)];
  }

 private:
  static constexpr std::size_t SIZE = MapSize;
  std::array<Value, SIZE> array;
  std::array<bool, SIZE> assigned;  // 디버깅을 위한 멤버
};
#endif
