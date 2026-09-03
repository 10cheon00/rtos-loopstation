#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#include <cstdbool>

#include "enum_raw.h"

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
inline EnumRaw ConvertEnumToEnumRaw(Enum id) {
  return static_cast<EnumRaw>(id);
}
template <typename Enum>
bool ConvertEnumRawToEnum(EnumRaw raw, Enum* id) {
  if (raw >= static_cast<EnumRaw>(Enum::COUNT)) {
    return false;
  }
  *id = static_cast<Enum>(raw);
  return true;
}

#endif
