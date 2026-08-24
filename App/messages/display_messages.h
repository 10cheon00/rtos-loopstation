#ifndef DISPLAY_MESSAGES_H
#define DISPLAY_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "ui_state.h"
#include "ui_state_id.h"
#include "button_id.h"
#include "parameter.h"
#include "track_state.h"
#include "ui_state_slot_index.h"
#include "menu_descriptor.h"

#define DISPLAY_COMMAND_QUEUE_TIMEOUT_500MS (500UL)

typedef struct {
    Parameter parameter;
    const char *label;
} ParameterRenderPayload;

typedef struct {
    MenuIconId icon_id;
    const char *label;
} MenuRenderPayload;

typedef struct {
    PanelSlotType type;
    union {
        MenuRenderPayload menu;
        ParameterRenderPayload parameter;
    } data;
} PanelSlotRenderPayload;

typedef struct {
    UiStateId ui_state_id;
    PanelSlotRenderPayload slot_render_payloads[UI_STATE_SLOT_INDEX_COUNT]; 
} PanelRenderPayload;

typedef struct {
    // TODO:
    // LED와 관련된 설정 구현하기
    Parameter ifx_a_state;
    Parameter tfx_a_state;
    TrackStateId track_state[TRACK_COUNT];
} LedRenderPayload;

typedef struct {
    PanelRenderPayload panel;
    LedRenderPayload led;
} DisplaySnapshot;

#endif
