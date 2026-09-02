#include "adc_input_task.h"

#include "FreeRTOS.h"
#include "adc_input_initparams.h"
#include "adc_rank_knob_config_table.h"
#include "cmsis_os2.h"
#include "input_messages.h"
#include "stm32h7xx.h"

#define ADC_POLLING_FREQEUNCY_HZ (100UL)
#define ADC_POLLING_DELAY_MS (1000UL / ADC_POLLING_FREQEUNCY_HZ)
#define ADC_POLLING_DELAY_TICKS (pdMS_TO_TICKS(ADC_POLLING_DELAY_MS))

static AdcValue_t adc_values[KNOB_ID_COUNT];
static ADC_HandleTypeDef* hadc;
static osMessageQueueId_t input_message_queue = 0;

static void ScanAllAdcAndSendMessages(AdcRank_t adc_rank, KnobId knob_id);

void AdcInputTask_Init(void* arguments) {
  AdcInputInitParams* params = (AdcInputInitParams*)arguments;

  hadc = params->hadc;
  input_message_queue = params->input_message_queue;

  AdcInputTask_Run();
  // for (;;) {
  //     osDelay(1);
  // }
}

void AdcInputTask_Run() {
  TickType_t last_wake_ticks = 0, next_wake_ticks;

  for (;;) {
    next_wake_ticks = last_wake_ticks + ADC_POLLING_DELAY_TICKS;
    osDelayUntil(next_wake_ticks);
    last_wake_ticks = osKernelGetTickCount();

    HAL_ADC_Start(hadc);
    AdcRankKnobConfigTable_Foreach(ScanAllAdcAndSendMessages);
    HAL_ADC_Stop(hadc);
  }
}

void ScanAllAdcAndSendMessages(AdcRank_t adc_rank, KnobId knob_id) {
  InputEvent input_event;

  HAL_ADC_PollForConversion(hadc, ADC_POLLING_DELAY_MS);
  adc_values[adc_rank] = HAL_ADC_GetValue(hadc);

  input_event.type = INPUT_EVENT_ADC_CONVERSION;
  input_event.payload.adc_conversion_event = (AdcConversionEvent){
      .timestamp_ticks = osKernelGetTickCount(),
      .adc_value = adc_values[adc_rank],
      .knob_id = knob_id,
  };
  osMessageQueuePut(input_message_queue, &input_event, 0,
                    INPUT_EVENT_QUEUE_TIMEOUT_500MS);
}
