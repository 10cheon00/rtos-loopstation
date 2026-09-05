#ifndef MUTABLE_ENUM_MAP_HPP
#define MUTABLE_ENUM_MAP_HPP

#include <array>
#include <optional>

template <typename Key, typename Value,
          std::size_t MapSize = static_cast<std::size_t>(Key::COUNT)>
class MutableEnumMap {
 public:
  template <typename... Entries>
  /**
   * 내부 array를 초기화하기 위해 가변 템플릿을 사용해 Entry들을 받은 후 하나씩
   * array에 대입한다.
   */
  constexpr explicit MutableEnumMap(Entries... entries) : array{}, assigned{} {
    ((array[static_cast<std::size_t>(entries.key)] = entries.value,
      assigned[static_cast<std::size_t>(entries.key)] = true),
     ...);
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
