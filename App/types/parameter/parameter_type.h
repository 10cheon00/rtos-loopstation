#ifndef PARAMETER_TYPE_H
#define PARAMETER_TYPE_H

typedef enum {
  NONE = 0,
  TOGGLE,
  UNSIGNED_RANGE,  // 0 ~ 100
  SIGNED_RANGE,    // -50 ~ 50
  DECIMAL_RANGE,   // 0.0 ~ 10.0
  MEASURE,
  ENUM,
} ParameterType;

#endif
