#ifndef DISPLAY_MESSAGES_H
#define DISPLAY_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "rtos_enum_value.h"
#include "rtos_parameter_copy.h"
#include "track_config.h"

#define DISPLAY_COMMAND_QUEUE_TIMEOUT_500MS (500UL)

typedef struct {
  RtosParameterCopy rtos_parameter_copy;
  const char* label;
} RtosPayload_ParameterRender;

typedef struct {
  RtosEnumValue16 rtos_enum_value16_menu_icon_encoding;
  const char* label;
} RtosPayload_MenuRender;

typedef struct {
  RtosEnumValue rtos_enum_value_page_slot_type;
  union {
    RtosPayload_MenuRender menu;
    RtosPayload_ParameterRender parameter;
  } data;
} RtosPayload_PageSlotRender;

typedef struct {
  RtosEnumValue rtos_enum_value_ui_state;
  RtosEnumValue rtos_enum_value_page_navigation_flag;
  RtosPayload_PageSlotRender page_slots[4];
} RtosPayload_PanelRender;

typedef struct {
  // TODO:
  // LED와 관련된 설정 구현하기
  RtosParameterCopy ifx_a_state;
  RtosParameterCopy tfx_a_state;
  RtosEnumValue rtos_enum_value_track_states[TRACK_COUNT];
} RtosPayload_LedRender;

typedef struct {
  RtosPayload_PanelRender panel;
  RtosPayload_LedRender led;
} RtosMessage_DisplaySnapshot;

#endif
