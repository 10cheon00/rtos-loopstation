#ifndef AUDIO_MESSAGE_H
#define AUDIO_MESSAGE_H

#include "rtos_enum_value.h"
#include "rtos_parameter_copy.h"
#include "track_config.h"

typedef struct {
  RtosEnumValue rtos_enum_value_track_states[TRACK_COUNT];

  RtosParameterCopy rtos_parameter_copy_ifx_a;
  RtosParameterCopy rtos_parameter_copy_ifx_b;
  RtosParameterCopy rtos_parameter_copy_ifx_c;

  RtosParameterCopy rtos_parameter_copy_tfx_a;
  RtosParameterCopy rtos_parameter_copy_tfx_b;
  RtosParameterCopy rtos_parameter_copy_tfx_c;
} RtosMessage_AudioEventSnapshot;

typedef struct {
  RtosEnumValue rtos_enum_value_audio_dma_event;
} RtosMessage_AudioDmaEvent;

#endif
