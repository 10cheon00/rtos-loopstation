#include "display_task.h"

#include "cmsis_os2.h"

#include "display_messages.h"
#include "display_initparams.h"
#include "ui_panel_renderer.h"

static u8g2_t u8g2;

static osMessageQueueId_t display_command_queue;

static TaskStatus
DisplayTask_HandleUiStateRenderPayload(UiStateRenderPayload *ui_state_render_payload);

static int DisplayTask_IsValidInitParams(const DisplayInitParams *params)
{
    return (params != 0) &&
           (params->display_command_queue != 0 && params->hspi != NULL && params->CS_Pin != 0 &&
            params->CS_Port != NULL && params->RST_Pin != 0 && params->RST_Port != NULL &&
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

    display_command_queue = params->display_command_queue;

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
    DisplayCommand command;
    for (;;) {
        osMessageQueueGet(display_command_queue, &command, NULL, osWaitForever);
        if (command.type == DISPLAY_COMMAND_UI_STATE_RENDER) {
            DisplayTask_HandleUiStateRenderPayload(&command.payload.ui_state_render);
        }
    }
}

TaskStatus DisplayTask_HandleUiStateRenderPayload(UiStateRenderPayload *ui_state_render_payload)
{
    UI_DRAWING_STATUS ui_drawing_status;

    ui_drawing_status = ui_panel_render_mappings[ui_state_render_payload->panel_id](
        &u8g2, ui_state_render_payload->parameter);

    if (ui_drawing_status != UI_DRAWING_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    return TASK_STATUS_OK;
}
