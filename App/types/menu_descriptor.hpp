#ifndef MENU_DESCRIPTOR_HPP
#define MENU_DESCRIPTOR_HPP

#include "menu_icon_encoding.hpp"
#include "ui_state_id.hpp"

typedef struct {
  MenuIconEncoding icon_encoding;
  UiStateMachine::Id state_id;
  const char* label;
} MenuDescriptor;

#endif
