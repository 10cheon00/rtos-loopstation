#ifndef UI_STATE_ID_H
#define UI_STATE_ID_H

#include <cstdint>

#include "id.h"

enum class UiStateId : std::uint8_t {
  NONE = ID_NONE,
  HOME,
  SETTING,
  SYSTEM_SETTING,
  COUNT  // 이 enum은 UiStateId 개수를 표시하는 용도임
};

#endif
