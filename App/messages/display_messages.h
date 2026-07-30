#ifndef DISPLAY_MESSAGES_H
#define DISPLAY_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "ui_panel_id.h"
#include "parameter.h"

#define DISPLAY_COMMAND_QUEUE_TIMEOUT_500MS_TO_TICKS (pdMS_TO_TICKS(500UL))

typedef enum {
  DISPLAY_COMMAND_UI_STATE_RENDER = 0
} DisplayCommandType;

typedef struct {
  UiPanelId panel_id;
  Parameter parameter[4];
} UiStateRenderPayload;

typedef struct {
  DisplayCommandType type;
  union {
    UiStateRenderPayload ui_state_render;
  } payload;
} DisplayCommand;

#endif
