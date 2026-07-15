#include "display_task.h"

#include "cmsis_os2.h"
#include "gmg12864_lcd.h"

#include "display_initparams.h"

static u8g2_t u8g2;
extern const uint8_t u8g2_font_ref4x5_prop_v4_tr[];

static int DisplayTask_IsValidInitParams(const DisplayInitParams *params)
{
    return (params != 0) && (params->display_command_queue != 0 && params->hspi != NULL &&
                             params->CS_Pin != 0 && params->CS_Port != NULL &&
                             params->RST_Pin != 0 && params->RST_Port != NULL &&
                             params->DC_Pin != 0 && params->DC_Port != NULL);
}

void DisplayTask_Init(void *argument)
{
    const DisplayInitParams *params = (const DisplayInitParams *)argument;

    if (!DisplayTask_IsValidInitParams(params)) {
        // TODO: 초기화 단계에서 오류 발생 시 처리 흐름에 대해 요구사항에서 정의하기
        for (;;) {
            osDelay(1);
        }
    }

    Gmg12864Lcd_InitParams initparams = {
        .hspi = params->hspi,
        .CS_Pin = params->CS_Pin,
        .CS_Port = params->CS_Port,
        .DC_Pin = params->DC_Pin,
        .DC_Port = params->DC_Port,
        .RST_Pin = params->RST_Pin,
        .RST_Port = params->RST_Port
    };
    ;

    Gmg12864LcdStatus status = Gmg12864Lcd_Init(&u8g2, &initparams);
    if (status != GMG12864_LCD_STATUS_OK) {
        // TODO: 초기화 단계에서 오류 발생 시 처리 흐름에 대해 요구사항에서 정의하기
        for (;;) {
            osDelay(1);
        }
    }

    DisplayTask_Run();
}

void DisplayTask_Run(void)
{
    u8g2_SetFont(&u8g2, u8g2_font_ref4x5_prop_v4_tr);
    u8g2_ClearBuffer(&u8g2);
    u8g2_SendBuffer(&u8g2);
    u8g2_DrawStr(&u8g2, 6, 6, "hi!");
    for (;;) {
    }
}
