#ifndef DISPLAY_MESSAGES_H
#define DISPLAY_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "enum_raw.h"
#include "panel_slot_type.h"
#include "parameter.h"
#include "track_config.h"

#define DISPLAY_COMMAND_QUEUE_TIMEOUT_500MS (500UL)

typedef uint8_t PageNavigationFlag;
enum {
  PAGE_NAVIGATION_FLAG_NONE = 0x0,
  PAGE_NAVIGATION_FLAG_LEFT_ARROW = 0x1,
  PAGE_NAVIGATION_FLAG_RIGHT_ARROW = 0x2,
};

typedef struct {
  Parameter parameter;
  const char* label;
} ParameterRenderPayload;

typedef struct {
  EnumRaw16 menu_icon_encoding_raw16;
  const char* label;
} MenuRenderPayload;

typedef struct {
  PanelSlotType type;
  union {
    MenuRenderPayload menu;
    ParameterRenderPayload parameter;
  } data;
} PanelSlotRenderPayload;

typedef struct {
  EnumRaw ui_state_enum_raw;
  PageNavigationFlag page_navigation_flag;
  PanelSlotRenderPayload slot_render_payloads[4];
} PanelRenderPayload;

/**
 * enum class로 구현된 TrackStateId를 C로 잠시 변환하는 DTO 역할을 맡기 위해
 * 정의된 타입
 */

typedef struct {
  // TODO:
  // LED와 관련된 설정 구현하기
  Parameter ifx_a_state;
  Parameter tfx_a_state;
  EnumRaw track_state_enum_raws[TRACK_COUNT];
} LedRenderPayload;

typedef struct {
  PanelRenderPayload panel;
  LedRenderPayload led;
} DisplaySnapshot;

#endif
