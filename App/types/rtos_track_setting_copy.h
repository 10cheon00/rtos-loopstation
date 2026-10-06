#ifndef RTOS_TRACK_SETTING_COPY_H
#define RTOS_TRACK_SETTING_COPY_H

#include <stdint.h>

#include "rtos_enum_value.h"

typedef int16_t RtosTrackSettingValue;

typedef struct {
  RtosTrackSettingValue min;
  RtosTrackSettingValue max;
  RtosTrackSettingValue current;
  RtosEnumValue track_setting_type_raw;
} RtosTrackSettingCopy;

#endif
