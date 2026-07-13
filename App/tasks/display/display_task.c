#include "display_task.h"

#include "cmsis_os2.h"
#include "gmg12864_lcd.h"

#include "display_initparams.h"

static Gmg12864LcdHandle_t handle;

static int DisplayTask_IsValidInitParams(const DisplayInitParams *params)
{
    return (params != 0) && (params->display_command_queue != 0 && params->hspi != NULL);
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
    Gmg12864LcdStatus status = Gmg12864Lcd_Init(&handle, &initparams);
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
