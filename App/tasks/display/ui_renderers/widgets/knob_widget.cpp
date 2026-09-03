#include "knob_widget.hpp"

#include <stdlib.h>

#include "utils.h"

namespace UiWidget {

#define RADIUS 7
#define BOX_WIDTH 2
#define BOX_HEIGHT 2

static int16_t ConvertParameterToDegree(Parameter* parameter);
static void DrawKnobIndicator(u8g2_t* u8g2, int16_t degree, uint8_t x,
                              uint8_t y);

void DrawKnobWidget(u8g2_t* u8g2, uint8_t x, uint8_t y, Parameter* parameter) {
  /**
   * 바늘지시식으로 노브의 값을 보여주어야 함.
   * 1. 그러므로 일단 노브의 값을 수학적으로 표현하는 각도로 변환
   *      min:current:max = angle_min:angle:angle_max
   *                        -45 : x : 225
   * 2. 각도에 따른 상대 좌표를 구함
   * 3. lcd 좌표 축에 맞추어 y를 뒤집음
   * 4. 중심 좌표에 더해서 최종 좌표를 구한다.
   */
  uint8_t cx, cy;
  int16_t degree;

  if (parameter->type != PARAMETER_TYPE_SLIDER) {
    return;
  }

  degree = ConvertParameterToDegree(parameter);
  cx = x + RADIUS + 1;
  cy = y + RADIUS;

  // 배경 렌더링
  u8g2_DrawDisc(u8g2, cx, cy, RADIUS, U8G2_DRAW_ALL);
  u8g2_DrawBox(u8g2, cx + RADIUS, cy + RADIUS, BOX_WIDTH, BOX_HEIGHT);
  u8g2_DrawBox(u8g2, cx - RADIUS - 1, cy + RADIUS, BOX_WIDTH, BOX_HEIGHT);

  // 노브 인디케이터 렌더링
  DrawKnobIndicator(u8g2, degree, cx, cy);
}

static int16_t ConvertParameterToDegree(Parameter* parameter) {
  int16_t degree;

  // 수학적 각도가 증가하는 방향과 파라미터가 증가하는 방향이 서로
  // 반대방향이므로 방향을 일치시키기 위해 계산
  degree = parameter->max - parameter->current + parameter->min;
  // 파라미터 값을 수학적 각도로 변환
  return (((double)degree - parameter->min) /
          (parameter->max - parameter->min)) *
             270.0 -
         45;
}

/**
 * Bresenham알고리즘으로 직선을 그려 노브 인디케이터를 렌더링
 */
static void DrawKnobIndicator(u8g2_t* u8g2, int16_t degree, uint8_t x,
                              uint8_t y) {
  int16_t dx, dy, sx, sy, err, x0, x1, y0, y1;
  x0 = x + cosine(degree) * 2;
  y0 = y - sine(degree) * 2;
  x1 = x + cosine(degree) * (RADIUS - 2);
  y1 = y - sine(degree) * (RADIUS - 2);
  dx = abs_int16(x1 - x0);
  sx = x0 < x1 ? 1 : -1;
  dy = -abs_int16(y1 - y0);
  sy = y0 < y1 ? 1 : -1;
  err = dx + dy;

  u8g2_SetDrawColor(u8g2, 0);
  while (1) {
    u8g2_DrawBox(u8g2, x0, y0, BOX_WIDTH, BOX_HEIGHT);
    if (x0 == x1 && y0 == y1) {
      break;
    }
    int16_t e2 = 2 * err;

    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }

    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
  u8g2_SetDrawColor(u8g2, 1);
}

}  // namespace UiWidget
