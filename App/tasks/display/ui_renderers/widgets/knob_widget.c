#include "knob_widget.h"

#include "utils.h"

#define RADIUS 7
#define BOX_WIDTH 2
#define BOX_HEIGHT 2

void UiWidget_DrawKnobWidget(u8g2_t* u8g2, uint8_t x, uint8_t y, Parameter* parameter) {
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
    int16_t angle;
    double x0, y0, x1, y1, x2, y2, x3, y3;
    
    if (parameter->type != PARAMETER_TYPE_SLIDER) {
        return;
    }
    
    // 수학적 각도가 증가하는 방향과 파라미터가 증가하는 방향이 서로 반대방향이므로
    // 방향을 일치시키기 위해 계산
    angle = parameter->max - parameter->current + parameter->min;
    // 파라미터 값을 수학적 각도로 변환
    angle = (((double)angle - parameter->min) / (parameter->max - parameter->min)) * 270 - 45;
    cx = x + RADIUS + 1;
    cy = y + RADIUS;
    x0 = cx + cosine(angle) * (RADIUS - 2);
    y0 = cy - sine(angle) * (RADIUS - 2);
    x3 = cx + cosine(angle) * 2;
    y3 = cy - sine(angle) * 2;
    x1 = 0.67 * x0 + 0.33 * x3; 
    y1 = 0.67 * y0 + 0.33 * y3; 
    y2 = 0.33 * y0 + 0.67 * y3; 
    x2 = 0.33 * x0 + 0.67 * x3; 
    // x0~3,y0~3이 사각형의 중심 좌표라고 생각하고, 좌상단이 되도록 보정하기
    if (x0 < 0) {
        x0 -= 1;
        x1 -= 1;
        x2 -= 1;
        x3 -= 1;
    } 
    if (y0 < 0) {
        y0 -= 1;
        y1 -= 1;
        y2 -= 1;
        y3 -= 1;
    }
    
    u8g2_DrawDisc(u8g2, cx, cy, RADIUS, U8G2_DRAW_ALL);
    u8g2_DrawBox(u8g2, cx + RADIUS, cy + RADIUS, BOX_WIDTH, BOX_HEIGHT);
    u8g2_DrawBox(u8g2, cx - RADIUS - 1, cy + RADIUS, BOX_WIDTH, BOX_HEIGHT);
    u8g2_DrawBox(u8g2, x0, y0, BOX_WIDTH, BOX_HEIGHT);
    u8g2_SetDrawColor(u8g2, 0);
    u8g2_DrawBox(u8g2, x0, y0, BOX_WIDTH, BOX_HEIGHT);
    u8g2_DrawBox(u8g2, x1, y1, BOX_WIDTH, BOX_HEIGHT);
    u8g2_DrawBox(u8g2, x2, y2, BOX_WIDTH, BOX_HEIGHT);
    u8g2_DrawBox(u8g2, x3, y3, BOX_WIDTH, BOX_HEIGHT);
    u8g2_SetDrawColor(u8g2, 1);
}
