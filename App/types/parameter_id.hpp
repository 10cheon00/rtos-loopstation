#ifndef PARAMETER_ID_HPP
#define PARAMETER_ID_HPP

#include <cstdint>

#include "id.h"

enum class ParameterId : std::uint8_t {
  NONE = ID_NONE,
  SETTING_PANEL_CURSOR,
  IFX_KNOB,
  TFX_KNOB,
  IFX_A_STATE,
  TFX_A_STATE,
  TRACK_1_VOLUME,
  SYSTEM_SETTING_LCD_CONSTRAST,
  COUNT,  // 이 enum은 ParameterId의 개수를 표시하는 용도임
};

#endif