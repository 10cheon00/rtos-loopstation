#include "input_task.h"

#include "cmsis_os2.h"

#include "input_initparams.h"
#include "input_messages.h"
#include "state_messages.h"
#include "mcp23017.h"
#include "mcp23017_gpio_map.h"

static osMessageQueueId_t input_event_queue;
static osMessageQueueId_t state_event_queue;
static I2C_HandleTypeDef *hi2c;
static osMutexId_t i2c1_mutex;

static TaskStatus HandleInputEvent(InputEvent *input_event);
static TaskStatus HandleMcp23017IntEvent(Mcp23017IntEvent *intEvent);
static TaskStatus GetPinState(Mcp23017Address address, Mcp23017GpioPinMask *gpio_a_pin_mask,
                              Mcp23017GpioPinMask *gpio_b_pin_mask,
                              Mcp23017GpioPinMask *gpio_a_state, Mcp23017GpioPinMask *gpio_b_state);
static TaskStatus SendButtonPayload(Mcp23017Address address, Mcp23017GpioId gpio_id,
                                    ButtonState button_state, TickType_t timestamp_ticks);
static TaskStatus HandleEncoderRotationEvent(EncoderRotationEvent *encoder_rotation_event);
static TaskStatus HandleAdcConversionEvent(AdcConversionEvent *adc_conversion_event);

static InputTaskContext input_task_context;

static int IsValidInitParams(const InputInitParams *params)
{
    return (params != 0) && (params->input_event_queue != 0 && params->state_event_queue != 0 &&
                             params->hi2c != NULL && params->i2c1_mutex != NULL);
}

void InputTask_Init(void *argument)
{
    const InputInitParams *params = (const InputInitParams *)argument;

    if (!IsValidInitParams(params)) {
        for (;;) {
            osDelay(1);
        }
    }

    input_event_queue = params->input_event_queue;
    state_event_queue = params->state_event_queue;
    hi2c = params->hi2c;
    i2c1_mutex = params->i2c1_mutex;

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
            task_status = HandleInputEvent(&input_event);
        }
    }
}

static TaskStatus HandleInputEvent(InputEvent *input_event)
{
    if (input_event->type == INPUT_EVENT_MCP23017) {
        return HandleMcp23017IntEvent(&input_event->payload.mcp23017_int_event);
    } else if (input_event->type == INPUT_EVENT_ENCODER_ROTATION) {
        return HandleEncoderRotationEvent(&input_event->payload.encoder_rotation_event);
    } else if (input_event->type == INPUT_EVENT_ADC_CONVERSION) {
        return HandleAdcConversionEvent(&input_event->payload.adc_conversion_event);
    }
    return TASK_STATUS_OK;
}

// 인터럽트가 발생한 핀을 확인하고 핀 상태를 얻어낸다.
// 핀 상태에 따라 상태 관리 태스크에게 메시지를 보낸다.
// TODO:  버튼 입력 이벤트를 debouncing하여 잘못된 입력을 전달하지 않도록 검사하기
static TaskStatus HandleMcp23017IntEvent(Mcp23017IntEvent *intEvent)
{
    TickType_t timestamp_ticks = intEvent->timestamp_ticks;
    Mcp23017Address address;
    Mcp23017GpioInterruptPin GPIO_Pin = intEvent->gpio_pin;
    Mcp23017GpioPinMask gpio_a_pin_mask, gpio_b_pin_mask;
    Mcp23017GpioPinMask gpio_a_state, gpio_b_state;

    Mcp23017GpioId gpio_id;
    ButtonId button_id;
    ButtonState button_state;
    osStatus_t os_status;
    TaskStatus taskStatus;
    uint8_t index;

    Mcp23017_GetMcp23017AddressFromInterruptPin(GPIO_Pin, &address);
    if (address == 0) {
        return TASK_STATUS_ERROR;
    }

    os_status = osMutexAcquire(i2c1_mutex, 500UL);
    if (os_status != osOK) {
        return TASK_STATUS_ERROR;
    }
    // 한 MCP23017의 두 포트를 모두 조회하여 활성화된 여러 입력핀들을 모두 처리
    taskStatus =
        GetPinState(address, &gpio_a_pin_mask, &gpio_b_pin_mask, &gpio_a_state, &gpio_b_state);
    osMutexRelease(i2c1_mutex);
    if (taskStatus != TASK_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }

    index = 0;
    while (gpio_a_pin_mask != 0) {
        if ((gpio_a_pin_mask & 0x1) != 0) {
            gpio_id = Mcp23017GpioMap_GetMcp23017GpioId(address, MCP23017_GPIO_PORT_A, index);
            if (gpio_id == MCP23017_GPIO_ID_NONE) {
                return TASK_STATUS_ERROR;
            }
            button_state = (gpio_a_state & 0x1) ? BUTTON_STATE_RELEASED : BUTTON_STATE_PRESSED;
            SendButtonPayload(address, gpio_id, button_state, timestamp_ticks);
        }
        gpio_a_pin_mask >>= 1;
        gpio_a_state >>= 1;
        index++;
    }
    index = 0;
    while (gpio_b_pin_mask != 0) {
        if ((gpio_b_pin_mask & 0x1) != 0) {
            gpio_id = Mcp23017GpioMap_GetMcp23017GpioId(address, MCP23017_GPIO_PORT_B, index);
            if (gpio_id == MCP23017_GPIO_ID_NONE) {
                return TASK_STATUS_ERROR;
            }
            button_state = (gpio_b_state & 0x1) ? BUTTON_STATE_RELEASED : BUTTON_STATE_PRESSED;
            SendButtonPayload(address, gpio_id, button_state, timestamp_ticks);
        }
        gpio_b_pin_mask >>= 1;
        gpio_b_state >>= 1;
        index++;
    }
    return TASK_STATUS_OK;
}

static TaskStatus GetPinState(Mcp23017Address address, Mcp23017GpioPinMask *gpio_a_pin_mask,
                              Mcp23017GpioPinMask *gpio_b_pin_mask,
                              Mcp23017GpioPinMask *gpio_a_state, Mcp23017GpioPinMask *gpio_b_state)
{
    Mcp23017Status status;
    uint8_t capture;

    status = Mcp23017_ReadRegister(hi2c, address, MCP23017_CONTROL_REGISTER_INTFA, gpio_a_pin_mask);
    if (status != MCP23017_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }
    status = Mcp23017_ReadRegister(hi2c, address, MCP23017_CONTROL_REGISTER_INTFB, gpio_b_pin_mask);
    if (status != MCP23017_STATUS_OK) {
        return TASK_STATUS_ERROR;
    }

    if (*gpio_a_pin_mask != 0) {
        status =
            Mcp23017_ReadRegister(hi2c, address, MCP23017_CONTROL_REGISTER_INTCAPA, gpio_a_state);
        if (status != MCP23017_STATUS_OK) {
            return TASK_STATUS_ERROR;
        }
    }
    if (*gpio_b_pin_mask != 0) {
        status =
            Mcp23017_ReadRegister(hi2c, address, MCP23017_CONTROL_REGISTER_INTCAPB, gpio_b_state);
        if (status != MCP23017_STATUS_OK) {
            return TASK_STATUS_ERROR;
        }
    }

    return TASK_STATUS_OK;
}

static TaskStatus SendButtonPayload(Mcp23017Address address, Mcp23017GpioId gpio_id,
                                    ButtonState button_state, TickType_t timestamp_ticks)
{
    ButtonId button_id = Mcp23017GpioMap_Get(gpio_id);
    ButtonPayload payload = {
        .id = button_id, .state = button_state, .timestamp_ticks = timestamp_ticks};
    StateEvent state_event = {.type = STATE_EVENT_BUTTON, .payload = {.button = payload}};
    osMessageQueuePut(state_event_queue, &state_event, 0, STATE_EVENT_QUEUE_TIMEOUT_500MS);

    if (button_id == BUTTON_ID_ENCODER_A_PUSH) {
        input_task_context.encoder_button_state[ENCODER_ID_A] = button_state;
    }
    if (button_id == BUTTON_ID_ENCODER_B_PUSH) {
        input_task_context.encoder_button_state[ENCODER_ID_B] = button_state;
    }
    if (button_id == BUTTON_ID_ENCODER_C_PUSH) {
        input_task_context.encoder_button_state[ENCODER_ID_C] = button_state;
    }
    if (button_id == BUTTON_ID_ENCODER_D_PUSH) {
        input_task_context.encoder_button_state[ENCODER_ID_D] = button_state;
    }
    return TASK_STATUS_OK;
}

static TaskStatus HandleEncoderRotationEvent(EncoderRotationEvent *encoder_rotation_event)
{
    uint8_t encoder_id = encoder_rotation_event->encoder_id;
    int32_t delta = 1;
    if (encoder_rotation_event->direction == ENCODER_ROTATE_COUNTER_CLOCKWISE) {
        delta = -1;
    }
    if (input_task_context.encoder_button_state[encoder_id] == BUTTON_STATE_PRESSED) {
        delta *= 10;
    }
    StateEvent state_event = {
        .type = STATE_EVENT_ENCODER_ROTATION,
        .payload = {.encoder_rotation = {
                        .delta = delta,
                        .encoder_id = encoder_id,
                        .timestamp_ticks = encoder_rotation_event->timestamp_ticks,
                    }}};
    osMessageQueuePut(state_event_queue, &state_event, 0, STATE_EVENT_QUEUE_TIMEOUT_500MS);

    return TASK_STATUS_OK;
}

static TaskStatus HandleAdcConversionEvent(AdcConversionEvent *adc_conversion_event)
{
    StateEvent state_event = {
        .type = STATE_EVENT_ADC_CONVERSION,
        .payload = {.adc_conversion = {.timestamp_ticks = adc_conversion_event->timestamp_ticks,
                                       .knob_id = adc_conversion_event->knob_id,
                                       .adc_value = adc_conversion_event->adc_value}}};

    osMessageQueuePut(state_event_queue, &state_event, 0, STATE_EVENT_QUEUE_TIMEOUT_500MS);

    return TASK_STATUS_OK;
}
