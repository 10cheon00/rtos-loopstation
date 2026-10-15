#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdbool>
#include <cstdint>
#include <optional>
#include <type_traits>

#include "button_id.hpp"
#include "encoder_id.hpp"
#include "rtos_enum_value.h"
#include "slot_index.hpp"

namespace Utils {

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

int16_t AbsInt16(int16_t n);
float CosineFromDegrees(float degree);
float SineFromDegrees(float degree);

using Hash_t = std::uint32_t;
Hash_t djb2(const char* string);

double MapRangeLinear(double x, double x_min, double x_max, double min,
                      double max);
float RoundToInt16(float f);

template <typename Enum>
inline std::underlying_type_t<Enum> ToRtosEnumValue(Enum id) {
  return static_cast<std::underlying_type_t<Enum>>(id);
}

template <typename Enum>
Enum FromRtosEnumValue(std::underlying_type_t<Enum> rtos_enum_value) noexcept {
  static_assert(std::is_enum_v<Enum>);
  return static_cast<Enum>(rtos_enum_value);
}

constexpr SlotIndex ToSlotIndex(ButtonId id) {
  switch (id) {
    case ButtonId::ENCODER_A_PUSH:
      return SlotIndex::A;

    case ButtonId::ENCODER_B_PUSH:
      return SlotIndex::B;

    case ButtonId::ENCODER_C_PUSH:
      return SlotIndex::C;

    case ButtonId::ENCODER_D_PUSH:
      return SlotIndex::D;

    default:
      return SlotIndex::INVALID;
  }
}

constexpr SlotIndex ToSlotIndex(EncoderId id) {
  switch (id) {
    case EncoderId::A:
      return SlotIndex::A;

    case EncoderId::B:
      return SlotIndex::B;

    case EncoderId::C:
      return SlotIndex::C;

    case EncoderId::D:
      return SlotIndex::D;

    default:
      return SlotIndex::INVALID;
  }
}

}  // namespace Utils

#endif
