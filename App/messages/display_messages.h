#ifndef DISPLAY_MESSAGES_H
#define DISPLAY_MESSAGES_H

#include <stdint.h>

typedef enum {
  DISPLAY_COMMAND_UI_STATE_RENDER = 0
} DisplayCommandType;

typedef struct {
  uint16_t panel_id;
  uint16_t selected_track;
  uint16_t selected_item;
  uint32_t timestamp_ms;
} UiStateRenderPayload;

typedef struct {
  DisplayCommandType type;
  union {
    UiStateRenderPayload ui_state_render;
  } payload;
} DisplayCommand;

#endif
