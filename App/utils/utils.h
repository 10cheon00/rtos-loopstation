#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#include <cstdbool>

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

template <typename Id, typename IdRaw>
inline IdRaw ConvertIdToIdRaw(Id id) {
  return static_cast<IdRaw>(id);
}
template <typename IdRaw, typename Id>
bool ConvertIdRawToId(IdRaw raw, Id* id) {
  if (raw >= static_cast<IdRaw>(Id::COUNT)) {
    return false;
  }
  *id = static_cast<Id>(raw);
  return true;
}

#endif
