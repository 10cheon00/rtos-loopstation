#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

#include <cstdint>

#include "display_messages.h"
#include "gmg12864_lcd.hpp"
#include "menu_icon_encoding.hpp"
#include "page.hpp"
#include "page_navigation_flag.hpp"
#include "parameter.hpp"
#include "parameter_id.hpp"
#include "ui_state_id.hpp"

extern const uint8_t u8g2_font_ref4x5_prop_v4_tr[];

namespace UiRenderer {

enum class Status : std::uint8_t {
  OK = 0,
  ERROR,
};

Status DrawPanelLayout(const char* panel_name,
                       PageNavigationFlag flag);
Status DrawParameter(Parameter& parameter, const char* label,
                     SlotPosition slot_position);
Status DrawMenu(MenuIconEncoding icon_id, const char* label,
                SlotPosition slot_position);
}  // namespace UiRenderer

#endif
