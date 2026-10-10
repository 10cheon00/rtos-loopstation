#include "toggle_switch_widget.hpp"

#include "gmg12864_lcd.hpp"

namespace UiWidget {

#define TOGGLE_SWITCH_INDICATOR_WIDTH 4
#define TOGGLE_SWITCH_INDICATOR_HEIGHT 4
#define PADDING 1
#define TOGGLE_SWITCH_RBOX_WIDTH (TOGGLE_SWITCH_INDICATOR_WIDTH + PADDING * 2)
#define TOGGLE_SWITCH_RBOX_HEIGHT TOGGLE_SWITCH_WIDGET_HEIGHT

void DrawToggleSwitchWidget(uint8_t x, uint8_t y,
                            ParameterModel::ToggleValue& toggle_value) {
  Gmg12864::Driver& driver = Gmg12864::Driver::GetInstance();

  uint8_t indicator_cx, indicator_cy;

  driver.drawRBox(x, y, TOGGLE_SWITCH_RBOX_WIDTH, TOGGLE_SWITCH_RBOX_HEIGHT, 2);
  driver.setDrawColor(0);
  indicator_cx = x + PADDING;
  if (toggle_value.is_on) {
    indicator_cy = y + PADDING;
  } else {
    indicator_cy = y + TOGGLE_SWITCH_RBOX_HEIGHT -
                   (PADDING + TOGGLE_SWITCH_INDICATOR_HEIGHT);
  }
  driver.drawRBox(indicator_cx, indicator_cy, TOGGLE_SWITCH_INDICATOR_WIDTH,
                  TOGGLE_SWITCH_INDICATOR_HEIGHT, 2);
  driver.setDrawColor(1);
}

}  // namespace UiWidget
