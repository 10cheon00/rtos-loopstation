#ifndef AUDIO_MESSAGE_H
#define AUDIO_MESSAGE_H

#include "rtos_enum_value.h"
#include "rtos_parameter_copy.h"
#include "track_config.h"

typedef struct {
  RtosEnumValue rtos_enum_value_track_states[TRACK_COUNT];
  RtosParameterValue rtos_parameter_track_volumes[TRACK_COUNT];
  // TODO:
  // 각 FX의 종류(Enum)를 추가하고, 활성화 상태와 FX의 모든 파라미터 값을 담은
  // 각 구조체들을 union으로 하는 멤버 추가
} RtosMessage_AudioEventSnapshot;

typedef struct {
  RtosEnumValue rtos_enum_value_audio_dma_event;
} RtosMessage_AudioDmaEvent;

#endif
