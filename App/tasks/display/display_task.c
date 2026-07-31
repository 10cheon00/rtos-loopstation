#include "display_task.h"

#include "cmsis_os2.h"

#include "display_messages.h"
#include "display_initparams.h"
#include "ui_panel_renderer.h"

#define DISPLAY_RENDER_FREQEUNCY_HZ (15UL)
#define DISPLAY_RENDER_DELAY_MS (1000UL / DISPLAY_RENDER_FREQEUNCY_HZ)
#define DISPLAY_RENDER_DELAY_TICKS (pdMS_TO_TICKS(DISPLAY_RENDER_DELAY_MS))

static u8g2_t u8g2;

static osMessageQueueId_t display_snapshot_mailbox;

static TaskStatus HandleUiStateRenderPayload(DisplaySnapshot *snapshot);

static int IsValidInitParams(const DisplayInitParams *params)
{
    return (params != 0) &&
           (params->display_snapshot_mailbox != 0 && params->hspi != NULL && params->CS_Pin != 0 &&
            params->CS_Port != NULL && params->RST_Pin != 0 && params->RST_Port != NULL &&
            params->DC_Pin != 0 && params->DC_Port != NULL);
}

void DisplayTask_Init(void *argument)
{
    const DisplayInitParams *params = (const DisplayInitParams *)argument;

    if (!IsValidInitParams(params)) {
        // TODO: 초기화 단계에서 오류 발생 시 처리 흐름에 대해 요구사항에서 정의하기
        for (;;) {
            osDelay(1);
        }
    }

    display_snapshot_mailbox = params->display_snapshot_mailbox;

    Gmg12864Lcd_InitParams initparams = {.hspi = params->hspi,
                                         .CS_Pin = params->CS_Pin,
                                         .CS_Port = params->CS_Port,
                                         .DC_Pin = params->DC_Pin,
                                         .DC_Port = params->DC_Port,
                                         .RST_Pin = params->RST_Pin,
                                         .RST_Port = params->RST_Port};

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
    TickType_t last_wake_ticks = 0, next_wake_ticks;
    DisplaySnapshot snapshot;

    for (;;) {
        next_wake_ticks = last_wake_ticks + DISPLAY_RENDER_DELAY_TICKS;
        osDelayUntil(next_wake_ticks);
        last_wake_ticks = osKernelGetTickCount();
        osMessageQueueGet(display_snapshot_mailbox, &snapshot, NULL, 0);
        HandleUiStateRenderPayload(&snapshot);
    }
}

static TaskStatus HandleUiStateRenderPayload(DisplaySnapshot *snapshot)
{
    UiPanelRenderFunction ui_panel_render_function;
    UiDrawingStatus ui_drawing_status;

    ui_panel_render_function =
        UiPanelRendererTable_GetUiPanelRenderFunction(snapshot->ui_state.panel_id);
    ui_drawing_status = ui_panel_render_function(&u8g2, snapshot->ui_state.parameters);
    // TODO:
    // LED의 상태를 바꾸는 기능 구현하기

    if (ui_drawing_status != UI_DRAWING_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    return TASK_STATUS_OK;
}
