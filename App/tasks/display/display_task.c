#include "display_task.h"

#include "cmsis_os2.h"
#include "u8g2.h"

#include "display_messages.h"
#include "display_initparams.h"
#include "mcp23017.h"
#include "mcp23017_gpio_map.h"
#include "ui_renderer.h"
#include "ui_state_label_config_table.h"

#define DISPLAY_RENDER_FREQEUNCY_HZ (100UL)
#define DISPLAY_RENDER_DELAY_MS (1000UL / DISPLAY_RENDER_FREQEUNCY_HZ)
#define DISPLAY_RENDER_DELAY_TICKS (pdMS_TO_TICKS(DISPLAY_RENDER_DELAY_MS))

static u8g2_t u8g2;
static osMessageQueueId_t display_snapshot_mailbox;
static I2C_HandleTypeDef *hi2c;
static osMutexId_t i2c1_mutex;

static uint8_t track_led_rgb_table[TRACK_STATE_ID_COUNT][TRACK_LED_COLOR_COUNT] = {
    [TRACK_STATE_ID_IDLE] = {0, 0, 0, 0},
    [TRACK_STATE_ID_RECORDING] = {0, UINT8_MAX, 0, 0},
    [TRACK_STATE_ID_STOPPED] = {0, 0, 0, UINT8_MAX},
    [TRACK_STATE_ID_PLAYING] = {0, 0, UINT8_MAX, 0},
    [TRACK_STATE_ID_OVERDUBBING] = {0, UINT8_MAX, UINT8_MAX, 0},
};

static TaskStatus HandlePanelRenderPayload(PanelRenderPayload *payload);
static TaskStatus HandleLedRenderPayload(LedRenderPayload *payload);
static TaskStatus RenderFxLed(Parameter *parameter, Mcp23017GpioId gpio_id);
static TaskStatus RenderTrackLed(TrackStateId state_id, uint8_t track_index);

static int IsValidInitParams(const DisplayInitParams *params)
{
    return (params != 0) &&
           (params->display_snapshot_mailbox != 0 && params->hi2c != 0 && params->hspi != NULL &&
            params->i2c1_mutex != NULL && params->CS_Pin != 0 && params->CS_Port != NULL &&
            params->RST_Pin != 0 && params->RST_Port != NULL && params->DC_Pin != 0 &&
            params->DC_Port != NULL);
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
    i2c1_mutex = params->i2c1_mutex;

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
        HandlePanelRenderPayload(&snapshot.panel);
        HandleLedRenderPayload(&snapshot.led);
    }
}

static TaskStatus HandlePanelRenderPayload(PanelRenderPayload *panel_render_payload)
{
    u8g2_ClearBuffer(&u8g2);

    const char *panel_label = UiStateLabelConfigTable_Get(panel_render_payload->ui_state_id);
    UI_DrawPanelLayout(&u8g2, panel_label, panel_render_payload->page_navigation_flag);

    for (uint8_t i = 0; i < UI_STATE_SLOT_INDEX_COUNT; i++) {
        PanelSlotRenderPayload *payload = &panel_render_payload->slot_render_payloads[i];
        if (payload->type == PANEL_SLOT_TYPE_MENU) {
            MenuRenderPayload *menu_render_payload = &payload->data.menu;
            UI_DrawMenu(&u8g2, menu_render_payload->icon_id, menu_render_payload->label,
                        (UiStateSlotIndex)i);
        } else if (panel_render_payload->slot_render_payloads[i].type ==
                   PANEL_SLOT_TYPE_PARAMETER) {
                       ParameterRenderPayload *parameter_render_payload = &payload->data.parameter;
            UI_DrawParameter(&u8g2, &parameter_render_payload->parameter,
                             parameter_render_payload->label, (UiStateSlotIndex)i);
        }
    }

    u8g2_SendBuffer(&u8g2);

    return TASK_STATUS_OK;
}

static TaskStatus HandleLedRenderPayload(LedRenderPayload *payload)
{
    if (RenderFxLed(&payload->ifx_a_state, MCP23017_GPIO_ID_LED_IFX_A) != TASK_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    if (RenderFxLed(&payload->tfx_a_state, MCP23017_GPIO_ID_LED_TFX_A) != TASK_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    for (uint8_t i = 0; i < TRACK_COUNT; i++) {
        if (RenderTrackLed(payload->track_state[i], i) != TASK_STATUS_OK) {
            return TASK_STATUS_ERROR;
        }
    }
    return TASK_STATUS_OK;
}

// ParameterId에 매핑된 address, port, 레지스터 상 핀의 비트 위치를 찾아야 함
// 현재 핀 상태에 따라 수정된 핀의 값을 Mcp23017 드라이버에게 넘겨 값을 업데이트하라고 함
static TaskStatus RenderFxLed(Parameter *parameter, Mcp23017GpioId gpio_id)
{
    ParameterPinMapEntry *entry;
    osStatus_t os_status;
    uint8_t pin_state, pin_register_mask;

    entry = Mcp23017GpioMap_GetEntry(gpio_id);
    if (entry == NULL) {
        return TASK_STATUS_ERROR;
    }
    pin_register_mask = 0x1 << entry->pin_index;
    pin_state = (parameter->current == parameter->max ? UINT8_MAX : 0) & pin_register_mask;
    os_status = osMutexAcquire(i2c1_mutex, 500UL);
    if (os_status != osOK) {
        return TASK_STATUS_ERROR;
    }
    if (Mcp23017_UpdateOutputPinState(hi2c, entry->address, entry->port, pin_register_mask,
                                      pin_state) != MCP23017_STATUS_OK) {
        os_status = osMutexRelease(i2c1_mutex);
        return TASK_STATUS_ERROR;
    }
    os_status = osMutexRelease(i2c1_mutex);

    return TASK_STATUS_OK;
}

static TaskStatus RenderTrackLed(TrackStateId state_id, uint8_t track_index)
{
    osStatus_t os_status;
    Mcp23017Status mcp23017_status;
    ParameterPinMapEntry *red_entry, *blue_entry, *green_entry;
    uint8_t red_pin_register_mask, green_pin_register_mask, blue_pin_register_mask;
    uint8_t red_register_output;
    uint8_t blue_register_output;
    uint8_t green_register_output;

    red_entry = Mcp23017GpioMap_GetTrackLedEntry(track_index, TRACK_LED_COLOR_RED);
    if (red_entry == NULL) {
        return TASK_STATUS_ERROR;
    }
    green_entry = Mcp23017GpioMap_GetTrackLedEntry(track_index, TRACK_LED_COLOR_GREEN);
    if (green_entry == NULL) {
        return TASK_STATUS_ERROR;
    }
    blue_entry = Mcp23017GpioMap_GetTrackLedEntry(track_index, TRACK_LED_COLOR_BLUE);
    if (blue_entry == NULL) {
        return TASK_STATUS_ERROR;
    }

    red_pin_register_mask = 1 << red_entry->pin_index;
    green_pin_register_mask = 1 << green_entry->pin_index;
    blue_pin_register_mask = 1 << blue_entry->pin_index;
    red_register_output =
        track_led_rgb_table[state_id][TRACK_LED_COLOR_RED] & red_pin_register_mask;
    green_register_output =
        track_led_rgb_table[state_id][TRACK_LED_COLOR_GREEN] & green_pin_register_mask;
    blue_register_output =
        track_led_rgb_table[state_id][TRACK_LED_COLOR_BLUE] & blue_pin_register_mask;

    os_status = osMutexAcquire(i2c1_mutex, 500UL);
    if (os_status != osOK) {
        return TASK_STATUS_ERROR;
    }
    mcp23017_status = Mcp23017_UpdateOutputPinState(hi2c, red_entry->address, red_entry->port,
                                                    red_pin_register_mask, red_register_output);
    if (mcp23017_status != MCP23017_STATUS_OK) {
        os_status = osMutexRelease(i2c1_mutex);
        return TASK_STATUS_ERROR;
    }
    mcp23017_status = Mcp23017_UpdateOutputPinState(hi2c, green_entry->address, green_entry->port,
                                                    green_pin_register_mask, green_register_output);
    if (mcp23017_status != MCP23017_STATUS_OK) {
        os_status = osMutexRelease(i2c1_mutex);
        return TASK_STATUS_ERROR;
    }
    mcp23017_status = Mcp23017_UpdateOutputPinState(hi2c, blue_entry->address, blue_entry->port,
                                                    blue_pin_register_mask, blue_register_output);
    if (mcp23017_status != MCP23017_STATUS_OK) {
        os_status = osMutexRelease(i2c1_mutex);
        return TASK_STATUS_ERROR;
    }
    os_status = osMutexRelease(i2c1_mutex);
    return TASK_STATUS_OK;
}
