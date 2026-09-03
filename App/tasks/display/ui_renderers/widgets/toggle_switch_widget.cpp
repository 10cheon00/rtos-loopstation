#include "toggle_switch_widget.hpp"

namespace UiWidget {

#define TOGGLE_SWITCH_INDICATOR_WIDTH 4
#define TOGGLE_SWITCH_INDICATOR_HEIGHT 4
#define PADDING 1
#define TOGGLE_SWITCH_RBOX_WIDTH (TOGGLE_SWITCH_INDICATOR_WIDTH + PADDING * 2)
#define TOGGLE_SWITCH_RBOX_HEIGHT TOGGLE_SWITCH_WIDGET_HEIGHT

void DrawToggleSwitchWidget(u8g2_t* u8g2, uint8_t x, uint8_t y,
                            Parameter* parameter) {
  uint8_t indicator_cx, indicator_cy;
  if (parameter->type != PARAMETER_TYPE_TOGGLE) {
    return;
  }

  u8g2_DrawRBox(u8g2, x, y, TOGGLE_SWITCH_RBOX_WIDTH, TOGGLE_SWITCH_RBOX_HEIGHT,
                2);
  u8g2_SetDrawColor(u8g2, 0);
  indicator_cx = x + PADDING;
  if (parameter->current == parameter->min) {
    indicator_cy = y + TOGGLE_SWITCH_RBOX_HEIGHT -
                   (PADDING + TOGGLE_SWITCH_INDICATOR_HEIGHT);
  } else {
    indicator_cy = y + PADDING;
  }
  u8g2_DrawRBox(u8g2, indicator_cx, indicator_cy, TOGGLE_SWITCH_INDICATOR_WIDTH,
                TOGGLE_SWITCH_INDICATOR_HEIGHT, 2);
  u8g2_SetDrawColor(u8g2, 1);
}

}  // namespace UiWidget
