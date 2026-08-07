#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

double cosine(int16_t angle);
double sine(int16_t angle);
#endif