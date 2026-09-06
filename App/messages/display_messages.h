#ifndef DISPLAY_MESSAGES_H
#define DISPLAY_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "enum_raw.h"
#include "parameter_raw.h"
#include "track_config.h"

#define DISPLAY_COMMAND_QUEUE_TIMEOUT_500MS (500UL)

typedef uint8_t PageNavigationFlag;
enum {
  PAGE_NAVIGATION_FLAG_NONE = 0x0,
  PAGE_NAVIGATION_FLAG_LEFT_ARROW = 0x1,
  PAGE_NAVIGATION_FLAG_RIGHT_ARROW = 0x2,
};

typedef struct {
  ParameterRaw parameter_raw;
  const char* label;
} ParameterRenderPayload;

typedef struct {
  EnumRaw16 menu_icon_encoding_raw16;
  const char* label;
} MenuRenderPayload;

typedef struct {
  EnumRaw page_slot_type_raw;
  union {
    MenuRenderPayload menu;
    ParameterRenderPayload parameter;
  } data;
} PageSlotRenderPayload;

typedef struct {
  EnumRaw ui_state_enum_raw;
  PageNavigationFlag page_navigation_flag;
  PageSlotRenderPayload slot_render_payloads[4];
} PanelRenderPayload;

typedef struct {
  // TODO:
  // LED와 관련된 설정 구현하기
  ParameterRaw ifx_a_state_raw;
  ParameterRaw tfx_a_state_raw;
  EnumRaw track_state_enum_raws[TRACK_COUNT];
} LedRenderPayload;

typedef struct {
  PanelRenderPayload panel;
  LedRenderPayload led;
} DisplaySnapshot;

#endif
