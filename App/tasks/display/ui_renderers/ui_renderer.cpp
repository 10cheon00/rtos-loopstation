#include "ui_renderer.hpp"

#include <array>

#include "knob_widget.hpp"
#include "toggle_switch_widget.hpp"
#include "utils.h"

namespace UiRenderer {

#define SLOT_WIDTH 32
#define CHARACTER_HEIGHT 5
#define PANEL_LABEL_HEIGHT (CHARACTER_HEIGHT + 1)
#define PANEL_LABEL_LINE_Y (PANEL_LABEL_HEIGHT + 1)
#define PARAMETER_PADDING 2
#define PARAMETER_VALUE_HEIGHT (CHARACTER_HEIGHT + 1 + CHARACTER_HEIGHT)
#define ICON_WIDTH 16
#define ICON_HEIGHT 16
#define LABEL_HEIGHT (CHARACTER_HEIGHT + 1 + CHARACTER_HEIGHT)
#define GRAPHIC_AREA_HEIGHT                                           \
  (SCREEN_HEIGHT -                                                    \
   (PANEL_LABEL_LINE_Y + PARAMETER_PADDING + PARAMETER_VALUE_HEIGHT + \
    PARAMETER_PADDING + PARAMETER_PADDING + LABEL_HEIGHT + 1))

/**
 * - 파라미터 출력
 * 값
 * 위젯 (그래픽 영역)
 * 라벨

 * - 메뉴 출력
 * (없음)
 * 아이콘 (그래픽 영역)
 * 라벨
 *
 * 좌표는 기본적으로 좌상단을 기준으로 출력한다.
 * 이미지나 글씨는 좌하단을 기준으로 출력해야하므로,
 *  기준 좌표를 좌상단으로 넘기면 출력을 처리하는 함수 내에서 좌하단으로
 변환하여 출력한다.
 */
static constexpr auto parameter_width_table = [] {
  std::array<uint8_t, UI_STATE_SLOT_INDEX_COUNT> values{};
  values[UI_STATE_SLOT_INDEX_A] = 0;
  values[UI_STATE_SLOT_INDEX_B] = SLOT_WIDTH;
  values[UI_STATE_SLOT_INDEX_C] = SLOT_WIDTH * 2;
  values[UI_STATE_SLOT_INDEX_D] = SLOT_WIDTH * 3;
  return values;
}();

static constexpr auto panel_menu_icon_table = [] {
  std::array<uint8_t, MENU_ICON_ID_COUNT> values{};
  values[MENU_ICON_ID_NONE] = 0;
  values[MENU_ICON_ID_SYSTEM] = 129;
  values[MENU_ICON_ID_DEBUG] = 104;
  return values;
}();

static void DrawArrowLeft4x5(u8g2_t* u8g2, uint8_t x, uint8_t y);
static void DrawArrowRight4x5(u8g2_t* u8g2, uint8_t x, uint8_t y);
static Status DrawParameterValue(u8g2_t* u8g2, Parameter* parameter, uint8_t x,
                                 uint8_t y);
static Status DrawParameterWidget(u8g2_t* u8g2, Parameter* parameter, uint8_t x,
                                  uint8_t y);
static void ConvertNumberToString(int32_t number, char* string,
                                  uint8_t string_length);
static Status DrawPanelMenuIcon(u8g2_t* u8g2, MenuIconId icon, uint8_t x,
                                uint8_t y);
static Status DrawLabel(u8g2_t* u8g2, const char* label, uint8_t x, uint8_t y);

// TODO:
// 패널 이동 화살표 표시도 자동화할 수 있지 않을까?
Status DrawPanelLayout(u8g2_t* u8g2, const char* panel_name,
                       PageNavigationFlag flag) {
  u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
  u8g2_ClearBuffer(u8g2);
  u8g2_DrawStr(u8g2, 1, PANEL_LABEL_HEIGHT, panel_name);
  u8g2_DrawLine(u8g2, 0, PANEL_LABEL_LINE_Y, SCREEN_WIDTH, PANEL_LABEL_LINE_Y);
  if (flag & PAGE_NAVIGATION_FLAG_LEFT_ARROW) {
    DrawArrowLeft4x5(u8g2, 117, PANEL_LABEL_HEIGHT);
  }
  if (flag & PAGE_NAVIGATION_FLAG_RIGHT_ARROW) {
    DrawArrowRight4x5(u8g2, 122, PANEL_LABEL_HEIGHT);
  }
  return Status::OK;
}

static void DrawArrowLeft4x5(u8g2_t* u8g2, uint8_t x, uint8_t y) {
  u8g2_DrawVLine(u8g2, x, y - 3, 1);
  u8g2_DrawVLine(u8g2, x + 1, y - 4, 3);
  u8g2_DrawVLine(u8g2, x + 2, y - 4, 3);
  u8g2_DrawVLine(u8g2, x + 3, y - 5, 5);
}

static void DrawArrowRight4x5(u8g2_t* u8g2, uint8_t x, uint8_t y) {
  u8g2_DrawVLine(u8g2, x, y - 5, 5);
  u8g2_DrawVLine(u8g2, x + 1, y - 4, 3);
  u8g2_DrawVLine(u8g2, x + 2, y - 4, 3);
  u8g2_DrawVLine(u8g2, x + 3, y - 3, 1);
}

Status DrawParameter(u8g2_t* u8g2, Parameter* parameter, const char* label,
                     UiStateSlotIndex index) {
  uint8_t x, y;
  Status status;

  if (index >= UI_STATE_SLOT_INDEX_COUNT) {
    return Status::ERROR;
  }

  x = parameter_width_table[index];
  y = PANEL_LABEL_LINE_Y + PARAMETER_PADDING;
  status = DrawParameterValue(u8g2, parameter, x, y);
  if (status != Status::OK) {
    return status;
  }

  x = parameter_width_table[index];
  y = PANEL_LABEL_LINE_Y + PARAMETER_PADDING + PARAMETER_VALUE_HEIGHT +
      PARAMETER_PADDING;
  status = DrawParameterWidget(u8g2, parameter, x, y);
  if (status != Status::OK) {
    return status;
  }

  x = parameter_width_table[index];
  y = SCREEN_HEIGHT - 1 - CHARACTER_HEIGHT - 1 - CHARACTER_HEIGHT;
  status = DrawLabel(u8g2, label, x, y);
  return status;
}

static Status DrawParameterValue(u8g2_t* u8g2, Parameter* parameter, uint8_t x,
                                 uint8_t y) {
  char str[5];
  uint8_t string_width;

  if (parameter->type == PARAMETER_TYPE_TOGGLE) {
    if (parameter->current == parameter->max) {
      str[0] = 'O';
      str[1] = 'N';
      str[2] = '\n';
    } else {
      str[0] = 'O';
      str[1] = 'F';
      str[2] = 'F';
      str[3] = '\n';
    }
    u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    string_width = u8g2_GetStrWidth(u8g2, str);
    u8g2_DrawStr(u8g2, x + SLOT_WIDTH / 2 - string_width / 2,
                 y + PARAMETER_VALUE_HEIGHT, str);
  } else if (parameter->type == PARAMETER_TYPE_SLIDER) {
    u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
    ConvertNumberToString(parameter->current, str, 5);
    string_width = u8g2_GetStrWidth(u8g2, str);
    u8g2_DrawStr(u8g2, x + SLOT_WIDTH / 2 - string_width / 2,
                 y + PARAMETER_VALUE_HEIGHT, str);
  } else {
    // TODO:
    // RATE_SLIDER면 4,2,1,1/2~1/16,0~100와 같이 출력해야함.
  }
  return Status::OK;
}

static Status DrawParameterWidget(u8g2_t* u8g2, Parameter* parameter, uint8_t x,
                                  uint8_t y) {
  if (parameter->type == PARAMETER_TYPE_TOGGLE) {
    x = x + SLOT_WIDTH / 2 - TOGGLE_SWITCH_WIDGET_WIDTH / 2;
    y = y + GRAPHIC_AREA_HEIGHT / 2 - TOGGLE_SWITCH_WIDGET_HEIGHT / 2;
    UiWidget::DrawToggleSwitchWidget(u8g2, x, y, parameter);
  } else if (parameter->type == PARAMETER_TYPE_SLIDER) {
    x = x + SLOT_WIDTH / 2 - KNOB_WIDGET_WIDTH / 2;
    y = y + GRAPHIC_AREA_HEIGHT / 2 - KNOB_WIDGET_HEIGHT / 2;
    UiWidget::DrawKnobWidget(u8g2, x, y, parameter);
  } else {
    // TODO:
    // RATE SLIDER형 파라미터 위젯 구현하기
  }
  return Status::OK;
}

static void ConvertNumberToString(int32_t number, char* string,
                                  uint8_t string_length) {
  uint8_t i = 0, j = 0, is_negative = number < 0 ? 1 : 0;
  char c;
  string[i] = '\0';
  i++;
  string[i] = '0';
  while (number != 0) {
    string[i] = number % 10 + '0';
    number /= 10;
    i++;
  }
  if (is_negative) {
    string[i] = '-';
    i++;
  }
  while (j < i - 1) {
    c = string[j];
    string[j] = string[i - 1];
    string[i - 1] = c;
    j++;
    i--;
  }
}

Status DrawMenu(u8g2_t* u8g2, MenuIconId icon_id, const char* label,
                UiStateSlotIndex index) {
  uint8_t x, y;
  if (index >= UI_STATE_SLOT_INDEX_COUNT) {
    return Status::ERROR;
  }

  x = parameter_width_table[index];
  y = PANEL_LABEL_LINE_Y + PARAMETER_PADDING + PARAMETER_VALUE_HEIGHT +
      PARAMETER_PADDING + GRAPHIC_AREA_HEIGHT / 2 - ICON_HEIGHT / 2;
  DrawPanelMenuIcon(u8g2, icon_id, x, y);

  x = parameter_width_table[index];
  y = SCREEN_HEIGHT - 1 - CHARACTER_HEIGHT - 1 - CHARACTER_HEIGHT;
  DrawLabel(u8g2, label, x, y);
  return Status::OK;
}

static Status DrawPanelMenuIcon(u8g2_t* u8g2, MenuIconId icon_id, uint8_t x,
                                uint8_t y) {
  uint8_t icon_encoding = panel_menu_icon_table[icon_id], glyph_width;
  u8g2_SetFont(u8g2, u8g2_font_open_iconic_all_2x_t);
  glyph_width = u8g2_GetGlyphWidth(u8g2, icon_encoding);
  u8g2_DrawGlyph(u8g2, x + SLOT_WIDTH / 2 - glyph_width / 2, y + ICON_HEIGHT,
                 icon_encoding);

  return Status::OK;
}

static Status DrawLabel(u8g2_t* u8g2, const char* label, uint8_t x, uint8_t y) {
  const char *first_line = label, *second_line;
  char* p = (char*)label;
  uint8_t string_width;
  while (1) {
    if (*p == '\0') {
      second_line = p;
      break;
    }
    if (*p == '\n') {
      second_line = p + 1;
      break;
    }
    p++;
  }

  u8g2_SetFont(u8g2, u8g2_font_ref4x5_prop_v4_tr);
  string_width = u8g2_GetStrWidth(u8g2, first_line);
  u8g2_DrawStr(u8g2, x + SLOT_WIDTH / 2 - string_width / 2,
               y + CHARACTER_HEIGHT, first_line);
  if (*second_line) {
    string_width = u8g2_GetStrWidth(u8g2, second_line);
    u8g2_DrawStr(u8g2, x + SLOT_WIDTH / 2 - string_width / 2,
                 y + CHARACTER_HEIGHT + 1 + CHARACTER_HEIGHT, second_line);
  }
  return Status::OK;
}

}  // namespace UiRenderer
