#ifndef ADC_RANK_KNOB_CONFIG_TABLE_H
#define ADC_RANK_KNOB_CONFIG_TABLE_H

#include "stm32h7xx.h"
#include "knob_id.h"

typedef uint8_t AdcRank_t;
typedef uint16_t AdcValue_t;
typedef void (*AdcRankKnobConfigMapCallbackFunction)(AdcRank_t adc_rank, KnobId knob_id);

void AdcRankKnobConfigMap_Foreach(AdcRankKnobConfigMapCallbackFunction CallbackFunction);

#endif