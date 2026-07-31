#include "display_task.h"

#include "cmsis_os2.h"

#include "display_messages.h"
#include "display_initparams.h"
#include "ui_panel_renderer.h"
#include "mcp23017.h"
#include "mcp23017_gpio_table.h"

#define DISPLAY_RENDER_FREQEUNCY_HZ (15UL)
#define DISPLAY_RENDER_DELAY_MS (1000UL / DISPLAY_RENDER_FREQEUNCY_HZ)
#define DISPLAY_RENDER_DELAY_TICKS (pdMS_TO_TICKS(DISPLAY_RENDER_DELAY_MS))

static u8g2_t u8g2;
static osMessageQueueId_t display_snapshot_mailbox;
static I2C_HandleTypeDef *hi2c;

static TaskStatus HandleUiStateRenderPayload(UiStateRenderPayload *payload);
static TaskStatus HandleLedRenderPayload(LedRenderPayload *payload);
static TaskStatus RenderLed(Parameter *parameter, Mcp23017GpioId gpio_id);

static int IsValidInitParams(const DisplayInitParams *params)
{
    return (params != 0) &&
           (params->display_snapshot_mailbox != 0 && params->hi2c != 0 && params->hspi != NULL &&
            params->CS_Pin != 0 && params->CS_Port != NULL && params->RST_Pin != 0 &&
            params->RST_Port != NULL && params->DC_Pin != 0 && params->DC_Port != NULL);
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
    hi2c = params->hi2c;

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
        HandleUiStateRenderPayload(&snapshot.ui_state);
        HandleLedRenderPayload(&snapshot.led);
    }
}

static TaskStatus HandleUiStateRenderPayload(UiStateRenderPayload *payload)
{
    UiPanelRenderFunction ui_panel_render_function;
    UiDrawingStatus ui_drawing_status;

    ui_panel_render_function = UiPanelRendererTable_GetUiPanelRenderFunction(payload->panel_id);
    ui_drawing_status = ui_panel_render_function(&u8g2, payload->parameters);

    if (ui_drawing_status != UI_DRAWING_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    return TASK_STATUS_OK;
}

static TaskStatus HandleLedRenderPayload(LedRenderPayload *payload)
{
    if (RenderLed(&payload->ifx_a_state, MCP23017_GPIO_ID_LED_IFX_A) != TASK_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    if (RenderLed(&payload->tfx_a_state, MCP23017_GPIO_ID_LED_TFX_A) != TASK_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }

    return TASK_STATUS_OK;
}

// ParameterId에 매핑된 address, port, 레지스터 상 핀의 비트 위치를 찾아야 함
// 현재 핀 상태에 따라 수정된 핀의 값을 Mcp23017 드라이버에게 넘겨 값을 업데이트하라고 함
static TaskStatus RenderLed(Parameter *parameter, Mcp23017GpioId gpio_id)
{
    ParameterPinMapEntry *entry;
    uint8_t pin_state, output;

    entry = Mcp23017GpioMap_GetEntry(gpio_id);
    if (entry == NULL) {
        return TASK_STATUS_ERROR;
    }
    output = parameter->current == parameter->max ? UINT8_MAX : 0;
    pin_state = entry->pin_register_mask & output;
    if (Mcp23017_UpdateOutputPinState(hi2c, entry->address, entry->port,
                                      entry->pin_register_mask, pin_state) !=
        MCP23017_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }

    return TASK_STATUS_OK;
}
