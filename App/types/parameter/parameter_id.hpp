#ifndef PARAMETER_ID_HPP
#define PARAMETER_ID_HPP

#include <cstdint>

#include "id.h"

namespace ParameterModel {

enum class Id : std::uint8_t {
  NONE = ID_NONE,
  IFX_KNOB,
  TFX_KNOB,
  TRACK_1_VOLUME,
  SYSTEM_SETTING_LCD_CONSTRAST,
  SELECTED_FX_SECTION,
  SELECTED_FX_SLOT,
  COUNT,  // 이 enum은 ParameterModel::Id의 개수를 표시하는 용도임
};

}

#endif