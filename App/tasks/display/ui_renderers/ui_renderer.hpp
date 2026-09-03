#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

#include <cstdint>

#include "display_messages.h"
#include "gmg12864_lcd.hpp"
#include "parameter.h"
#include "parameter_id.h"
#include "ui_state_id.h"

extern const uint8_t u8g2_font_ref4x5_prop_v4_tr[];

namespace UiRenderer {

enum class Status : std::uint8_t {
  OK = 0,
  ERROR,
};

// typedef Status (*UiPanelRenderFunction)(u8g2_t*, ParameterRenderPayload*);
using UiPanelRenderFunction = Status(u8g2_t*, ParameterRenderPayload*);

Status DrawPanelLayout(u8g2_t* u8g2, const char* panel_name,
                       PageNavigationFlag flag);
Status DrawParameter(u8g2_t* u8g2, Parameter* parameter, const char* label,
                     UiStateSlotIndex index);
Status DrawMenu(u8g2_t* u8g2, MenuIconId icon_id, const char* label,
                UiStateSlotIndex index);
}  // namespace UiRenderer

#endif
