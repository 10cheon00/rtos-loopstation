#ifndef BUTTON_STATE_H
#define BUTTON_STATE_H

#include <cstdint>

enum class ButtonState : std::uint8_t {
  RELEASED = 0,
  PRESSED,
  COUNT,
};

#endif
