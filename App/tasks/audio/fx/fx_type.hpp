#ifndef FX_TYPE_HPP
#define FX_TYPE_HPP

#include <array>

namespace Fx {

enum class Type {
  NONE,
  LPF,
  COUNT,
};

inline constexpr const char* TYPE_NAME_TABLE[] = {
    " ",
    "LPF",
};

}  // namespace Fx

#endif
