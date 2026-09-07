#ifndef DISPLAY_MESSAGES_H
#define DISPLAY_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "enum_raw.h"
#include "parameter_raw.h"
#include "track_config.h"

#define DISPLAY_COMMAND_QUEUE_TIMEOUT_500MS (500UL)

typedef uint8_t RtosPayload_PageNavigationFlag;
enum {
  PAGE_NAVIGATION_FLAG_NONE = 0x0,
  PAGE_NAVIGATION_FLAG_LEFT_ARROW = 0x1,
  PAGE_NAVIGATION_FLAG_RIGHT_ARROW = 0x2,
};

typedef struct {
  ParameterRaw parameter_raw;
  const char* label;
} RtosPayload_ParameterRender;

typedef struct {
  RtosEnumValue16 menu_icon_encoding_raw16;
  const char* label;
} RtosPayload_MenuRender;

typedef struct {
  RtosEnumValue page_slot_type_raw;
  union {
    RtosPayload_MenuRender menu;
    RtosPayload_ParameterRender parameter;
  } data;
} RtosPayload_PageSlotRender;

typedef struct {
  RtosEnumValue ui_state_enum_raw;
  RtosPayload_PageNavigationFlag page_navigation_flag;
  RtosPayload_PageSlotRender slot_render_payloads[4];
} RtosPayload_PanelRender;

typedef struct {
  // TODO:
  // LED와 관련된 설정 구현하기
  ParameterRaw ifx_a_state_raw;
  ParameterRaw tfx_a_state_raw;
  RtosEnumValue track_state_enum_raws[TRACK_COUNT];
} RtosPayload_LedRender;

typedef struct {
  RtosPayload_PanelRender panel;
  RtosPayload_LedRender led;
} RtosMessage_DisplaySnapshot;

#endif
