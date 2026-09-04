#ifndef MENU_ICON_ENCODING_HPP
#define MENU_ICON_ENCODING_HPP

#include "enum_id.hpp"

enum class MenuIconEncoding : EnumId16 {
  NONE = 0,
  MISSING = 0,
  SYSTEM = 129,
  HARDWARE_DEBUG = 104,
  COUNT,
};

#endif
