#ifndef ADC_RANK_KNOB_MAP_H
#define ADC_RANK_KNOB_MAP_H

#include "stm32h7xx.h"
#include "knob_id.h"

// TODO:
// 랭크 순으로 adc polling conversion이 수행되니까, 
// 테이블 형태로 만들고 채널에 연결된 노브 id를 적는 것으로 해야함

KnobId AdcRankKnobTable_GetKnobIdFromAdcRank(uint32_t adc_rank);

#endif