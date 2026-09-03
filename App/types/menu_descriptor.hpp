#ifndef MENU_DESCRIPTOR_HPP
#define MENU_DESCRIPTOR_HPP

#include "menu_icon_id.h"
#include "ui_state_id.h"

typedef struct {
  MenuIconId icon_id;
  UiStateId state_id;
  const char* label;
} MenuDescriptor;

#endif
