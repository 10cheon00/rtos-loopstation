#ifndef MENU_DESCRIPTOR_H
#define MENU_DESCRIPTOR_H

#include "ui_state_id.h"

typedef enum {
  MENU_ICON_ID_NONE = 0,
  MENU_ICON_ID_SYSTEM,
  MENU_ICON_ID_DEBUG,
  MENU_ICON_ID_COUNT,
} MenuIconId;

typedef struct {
  MenuIconId icon_id;
  UiStateId state_id;
  const char* label;
} MenuDescriptor;

#endif
