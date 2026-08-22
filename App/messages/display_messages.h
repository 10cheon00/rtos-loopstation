#ifndef DISPLAY_MESSAGES_H
#define DISPLAY_MESSAGES_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "ui_state_id.h"
#include "button_id.h"
#include "parameter.h"
#include "track_state.h"
#include "ui_state_slot_index.h"

#define DISPLAY_COMMAND_QUEUE_TIMEOUT_500MS (500UL)

typedef struct {
    Parameter parameter;
    const char *label;
} UiParameterRenderSlot;

typedef struct {
    UiStateId ui_state_id;
    UiParameterRenderSlot parameter_slots[UI_STATE_SLOT_INDEX_COUNT];
} UiStateRenderPayload;

typedef struct {
    // TODO:
    // LED와 관련된 설정 구현하기
    Parameter ifx_a_state;
    Parameter tfx_a_state;
    TrackStateId track_state[TRACK_COUNT];
} LedRenderPayload;

typedef struct {
    UiStateRenderPayload ui_state;
    LedRenderPayload led;
} DisplaySnapshot;

#endif
