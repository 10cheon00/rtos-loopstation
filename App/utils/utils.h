#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

#define CONCATENATE2_IMPL(A, B) A##B
#define CONCATENATE2(A, B) CONCATENATE2_IMPL(A, B)

double cosine(int16_t angle);
double sine(int16_t angle);
uint32_t djb2(const char *string);

#endif
