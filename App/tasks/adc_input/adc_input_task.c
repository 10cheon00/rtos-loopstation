#include "adc_input_task.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "stm32h7xx.h"

#include "adc_rank_knob_table.h"
#include "adc_input_initparams.h"
#include "input_messages.h"

#define ADC_POLLING_FREQEUNCY_HZ (100UL)
#define ADC_POLLING_DELAY_MS (1000UL / ADC_POLLING_FREQEUNCY_HZ)
#define ADC_POLLING_DELAY_TICKS (pdMS_TO_TICKS(ADC_POLLING_DELAY_MS))

static uint16_t adc_values[KNOB_ID_COUNT];
static ADC_HandleTypeDef* hadc;
static osMessageQueueId_t input_message_queue = 0;

void AdcInputTask_Init(void *arguments) {
    AdcInputInitParams* params = (AdcInputInitParams*)arguments;

    hadc = params->hadc;
    input_message_queue = params->input_message_queue;

    AdcInputTask_Run();
    // for (;;) {
    //     osDelay(1);
    // }
}

void AdcInputTask_Run() {
    TickType_t last_wake_ticks= 0, next_wake_ticks;
    InputEvent input_event;

    for (;;) {
        next_wake_ticks = last_wake_ticks + ADC_POLLING_DELAY_TICKS;
        osDelayUntil(next_wake_ticks);
        last_wake_ticks = osKernelGetTickCount();

        HAL_ADC_Start(hadc);
        for (size_t i = 0; i < adc_rank_knob_table_count; i++) {
            HAL_ADC_PollForConversion(hadc, ADC_POLLING_DELAY_MS);
            adc_values[i] = HAL_ADC_GetValue(hadc);

            input_event.type = INPUT_EVENT_ADC_CONVERSION;
            input_event.payload.adc_conversion_event = (AdcConversionEvent) {
                .timestamp_ticks = osKernelGetTickCount(),
                .adc_value = adc_values[i],
                .knob_id = adc_rank_knob_table[i],
            };
            osMessageQueuePut(input_message_queue, &input_event, 0, INPUT_EVENT_QUEUE_TIMEOUT_500MS);
        }
        HAL_ADC_Stop(hadc);
    }
}
