#include "display_task.h"

#include "cmsis_os2.h"
#include "gmg12864_lcd.h"

#include "display_initparams.h"

static u8g2_t u8g2;

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

    Gmg12864Lcd_InitParams initparams = {.hspi = params->hspi};
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
    for (;;) {
    }
}
