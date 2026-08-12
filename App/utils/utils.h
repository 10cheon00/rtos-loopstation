#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

#define CONCATENATE2_IMPL(A, B) A##B
#define CONCATENATE2(A, B) CONCATENATE2_IMPL(A, B)

#define VARIABLE_TO_STR_IMPL(A) #A
#define VARIABLE_TO_STR(A) VARIABLE_TO_STR_IMPL(A)

double cosine(int16_t angle);
double sine(int16_t angle);
typedef uint32_t Hash_t;
Hash_t djb2(const char *string);

#endif
