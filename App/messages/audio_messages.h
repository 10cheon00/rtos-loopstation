#ifndef AUDIO_MESSAGE_H
#define AUDIO_MESSAGE_H

#include "rtos_enum_value.h"
#include "rtos_parameter_copy.h"
#include "track_config.h"

typedef struct {
  RtosEnumValue rtos_enum_value_track_states[TRACK_COUNT];
  RtosParameterValue rtos_parameter_track_volumes[TRACK_COUNT];
} RtosMessage_AudioEventSnapshot;

typedef struct {
  RtosEnumValue rtos_enum_value_audio_dma_event;
} RtosMessage_AudioDmaEvent;

#endif
