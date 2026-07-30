#include "input_task.h"

#include "cmsis_os2.h"

#include "input_button_mapping.h"
#include "input_mcp23017_devices.h"
#include "input_initparams.h"
#include "input_messages.h"
#include "state_messages.h"
#include "mcp23017.h"

#define INPUT_TASK_TIMEOUT_MS 500
#define INPUT_TASK_TIMEOUT_TICKS (pdMS_TO_TICKS(INPUT_TASK_TIMEOUT_MS))

static osMessageQueueId_t input_event_queue;
static osMessageQueueId_t state_event_queue;
static I2C_HandleTypeDef *hi2c;

static TaskStatus InputTask_HandleInputEvent(InputEvent *input_event);
static TaskStatus InputTask_HandleMcp23017IntEvent(Mcp23017IntEvent *intEvent);
static TaskStatus InputTask_FindI2cSlaveAddress(uint16_t GPIO_Pin, uint8_t *address);
static TaskStatus InputTask_GetPinState(uint8_t address, uint16_t *button_id_mask,
                                        ButtonState *button_state);
static TaskStatus InputTask_FindButtonId(uint8_t address, uint16_t button_id_mask,
                                                ButtonId *button_id);
static TaskStatus InputTask_HandleEncoderRotationEvent(EncoderRotationEvent* encoder_rotation_event);

static InputTaskContext input_task_context;

static int InputTask_IsValidInitParams(const InputInitParams *params)
{
    return (params != 0) && (params->input_event_queue != 0 && params->state_event_queue != 0 &&
                             params->hi2c != NULL);
}

void InputTask_Init(void *argument)
{
    const InputInitParams *params = (const InputInitParams *)argument;

    if (!InputTask_IsValidInitParams(params)) {
        for (;;) {
            osDelay(1);
        }
    }

    input_event_queue = params->input_event_queue;
    state_event_queue = params->state_event_queue;
    hi2c = params->hi2c;

    Mcp23017InitParams mcp23017InitParams = {.hi2c = params->hi2c,
                                             .device_configs = input_mcp23017_devices,
                                             .device_config_count = input_mcp23017_device_count};
    Mcp23017Status mcp23017Status = Mcp23017_Init(&mcp23017InitParams);
    if (mcp23017Status != MCP23017_STATUS_OK) {
        for (;;) {
            osDelay(1);
        }
    }

    input_task_context.previous_encoder_counter = 0;

    InputTask_Run();
}

void InputTask_Run(void)
{
    TaskStatus task_status;
    InputEvent input_event;
    osStatus_t os_status;
    for (;;) {
        os_status = osMessageQueueGet(input_event_queue, &input_event, NULL, osWaitForever);
        if (os_status == osOK) {
            task_status = InputTask_HandleInputEvent(&input_event);
        }
    }
}

static TaskStatus InputTask_HandleInputEvent(InputEvent *input_event)
{
    if (input_event->type == INPUT_EVENT_MCP23017) {
        return InputTask_HandleMcp23017IntEvent(&input_event->payload.mcp23017_int_event);
    } else if (input_event->type == INPUT_EVENT_ENCODER_ROTATION) {
        return InputTask_HandleEncoderRotationEvent(&input_event->payload.encoder_rotation_event);
    }
    return TASK_STATUS_OK;
}

// 인터럽트가 발생한 핀을 확인하고 핀 상태를 얻어낸다.
// 핀 상태에 따라 상태 관리 태스크에게 메시지를 보낸다.
static TaskStatus InputTask_HandleMcp23017IntEvent(Mcp23017IntEvent *intEvent)
{
    TickType_t timestamp_ticks = intEvent->timestamp_ticks;
    uint16_t GPIO_Pin = intEvent->gpio_pin;
    uint16_t button_id_mask;
    uint8_t address;
    ButtonState button_state;
    ButtonId button_id;
    TaskStatus taskStatus;

    taskStatus = InputTask_FindI2cSlaveAddress(GPIO_Pin, &address);
    if (taskStatus != TASK_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }

    taskStatus = InputTask_GetPinState(address, &button_id_mask, &button_state);
    if (taskStatus != TASK_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    // TODO:  버튼 입력 이벤트를 debouncing하여 잘못된 입력을 전달하지 않도록 검사하기

    taskStatus = InputTask_FindButtonId(address, button_id_mask, &button_id);
    ButtonPayload payload = {
        .id = button_id,
        .state = button_state,
        .timestamp_ticks = timestamp_ticks
    };
    StateEvent state_event = {.type = STATE_EVENT_BUTTON,
                              .payload = {.button = payload}};
    osMessageQueuePut(state_event_queue, &state_event, 0, INPUT_TASK_TIMEOUT_TICKS);

    return TASK_STATUS_OK;
}

TaskStatus InputTask_FindI2cSlaveAddress(uint16_t GPIO_Pin, uint8_t *address)
{

    for (uint8_t i = 0; i < input_mcp23017_device_count; i++) {
        if (input_mcp23017_devices[i].gpio_pin == GPIO_Pin) {
            *address = input_mcp23017_devices[i].address;
            return TASK_STATUS_OK;
        }
    }
    *address = 0;
    return TASK_STATUS_ERROR;
}

static TaskStatus InputTask_GetPinState(uint8_t address, uint16_t *button_id_mask,
                                        ButtonState *button_state)
{
    Mcp23017Status status;

    uint8_t flag_a, flag_b;
    uint8_t capture_a, capture_b;
    status = Mcp23017_ReadRegister(hi2c, address, MCP23017_CONTROL_REGISTER_INTFA, &flag_a);
    if (status != MCP23017_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    status = Mcp23017_ReadRegister(hi2c, address, MCP23017_CONTROL_REGISTER_INTCAPA, &capture_a);
    if (status != MCP23017_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }

    if (flag_a) {
        *button_id_mask = flag_a << 0;
        *button_state =
            flag_a & capture_a ? BUTTON_STATE_RELEASED : BUTTON_STATE_PRESSED;
        return TASK_STATUS_OK;
    }

    status = Mcp23017_ReadRegister(hi2c, address, MCP23017_CONTROL_REGISTER_INTFB, &flag_b);

    if (status != MCP23017_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    status = Mcp23017_ReadRegister(hi2c, address, MCP23017_CONTROL_REGISTER_INTCAPB, &capture_b);
    if (status != MCP23017_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    if (flag_b) {
        *button_id_mask = flag_b << 8;
        *button_state =
            flag_b & capture_b ? BUTTON_STATE_RELEASED : BUTTON_STATE_PRESSED;
    }
    return TASK_STATUS_OK;
}

static TaskStatus InputTask_FindButtonId(uint8_t address, uint16_t button_id_mask,
                                                ButtonId *button_id)
{
    uint8_t mapping_index = 0;
    while (button_id_mask != 0 && (button_id_mask & 0x1) == 0) {
        button_id_mask >>= 1;
        mapping_index++;
    }
    for (uint8_t i = 0; i < input_button_mapping_count; i++) {
        if (input_button_mappings[i].slave_address == address) {
            *button_id = input_button_mappings[i].ids[mapping_index];
            return TASK_STATUS_OK;
        }
    }
    return TASK_STATUS_ERROR;
}

static TaskStatus InputTask_HandleEncoderRotationEvent(EncoderRotationEvent* encoder_rotation_event) {
    uint16_t current = encoder_rotation_event->encoder_counter;
    uint16_t previous = input_task_context.previous_encoder_counter;
    uint16_t unsigned_delta = (uint16_t)(current - previous);
    int32_t delta = (int32_t)unsigned_delta;
    if (unsigned_delta > INT16_MAX) {
        delta = (int32_t)unsigned_delta - 65536;
    }

    StateEvent state_event = {
        .type = STATE_EVENT_ENCODER_ROTATION,
        .payload = {
            .encoder_rotation = {
                .delta = delta,
                .encoder_id = encoder_rotation_event->encoder_id,
                .timestamp_ticks = encoder_rotation_event->timestamp_ticks,
            }
        }
    };
    input_task_context.previous_encoder_counter = encoder_rotation_event->encoder_counter;
    osMessageQueuePut(state_event_queue, &state_event, 0, INPUT_TASK_TIMEOUT_TICKS);
    
    return TASK_STATUS_OK;
}