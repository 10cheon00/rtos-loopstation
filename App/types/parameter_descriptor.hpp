#ifndef PARAMETER_DESCRIPTOR_H
#define PARAMETER_DESCRIPTOR_H

#include "parameter_id.hpp"

struct ParameterDescriptor {
  ParameterId id;
  const char* label;
};

#endif
