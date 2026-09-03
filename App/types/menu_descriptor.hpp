#ifndef MENU_DESCRIPTOR_H
#define MENU_DESCRIPTOR_H

#include "menu_icon_id.h"
#include "ui_state_id.h"

typedef struct {
  MenuIconId icon_id;
  UiStateId state_id;
  const char* label;
} MenuDescriptor;

#endif
