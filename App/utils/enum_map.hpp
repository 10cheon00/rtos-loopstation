#ifndef ENUM_MAP_H
#define ENUM_MAP_H

#include <array>
#include <cstddef>
#include <type_traits>

template <typename Key, typename Value>
struct EnumEntry {
  Key key;
  Value value;

  constexpr EnumEntry(Key key, Value value) : key(key), value(value) {}
};

/**
 * Enum을 Key로 사용하고 크기가 고정된 맵.
 * 고정된 크기를 구하기 위해 EnumMap에 사용되는 enum class가 COUNT를 가져야 함을
 * 전제로 한다.
 */
template <typename Key, typename Value,
          std::size_t MapSize = static_cast<std::size_t>(Key::COUNT)>
class EnumMap {
 public:
  template <typename... Entries>
  /**
   * 내부 array를 초기화하기 위해 가변 템플릿을 사용해 Entry들을 받은 후 하나씩
   * array에 대입한다.
   */
  constexpr explicit EnumMap(Entries... entries) : array{}, assigned{} {
    ((array[static_cast<std::size_t>(entries.key)] = entries.value,
      assigned[static_cast<std::size_t>(entries.key)] = true),
     ...);
  }

  constexpr const Value& Get(Key key) const {
    return array[static_cast<std::size_t>(key)];
  }

  constexpr const Value& operator[](Key key) const {
    return array[static_cast<std::size_t>(key)];
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
