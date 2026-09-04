#ifndef UTILS_H
#define UTILS_H

#include <cstdbool>
#include <cstdint>
#include <optional>
#include <type_traits>

#include "button_id.hpp"
#include "encoder_id.h"
#include "enum_raw.h"
#include "slot_position.hpp"

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

#define CONCATENATE2_IMPL(A, B) A##B
#define CONCATENATE2(A, B) CONCATENATE2_IMPL(A, B)

#define VARIABLE_TO_STR_IMPL(A) #A
#define VARIABLE_TO_STR(A) VARIABLE_TO_STR_IMPL(A)

double abs_double(double d);
int16_t abs_int16(int16_t n);
double cosine(int16_t degree);
double sine(int16_t degree);
double tangent(int16_t degree);
typedef uint32_t Hash_t;
Hash_t djb2(const char* string);

template <typename Enum>
inline std::underlying_type_t<Enum> ConvertEnumToRaw(Enum id) {
  return static_cast<std::underlying_type_t<Enum>>(id);
}

template <typename Enum>
bool ConvertEnumRawToEnum(std::underlying_type_t<Enum> raw, Enum* id) {
  if (id == nullptr) {
    return false;
  }
  *id = static_cast<Enum>(raw);
  return true;
}

constexpr std::optional<SlotPosition> ToSlotPosition(ButtonId id) {
  switch (id) {
    case ButtonId::ENCODER_A_PUSH:
      return SlotPosition::A;

    case ButtonId::ENCODER_B_PUSH:
      return SlotPosition::B;

    case ButtonId::ENCODER_C_PUSH:
      return SlotPosition::C;

    case ButtonId::ENCODER_D_PUSH:
      return SlotPosition::D;

    default:
      return std::nullopt;
  }
}

constexpr std::optional<SlotPosition> ToSlotPosition(EncoderId id) {
  switch (id) {
    case ENCODER_ID_A:
      return SlotPosition::A;

    case ENCODER_ID_B:
      return SlotPosition::B;

    case ENCODER_ID_C:
      return SlotPosition::C;

    case ENCODER_ID_D:
      return SlotPosition::D;

    default:
      return std::nullopt;
  }
}

#endif
