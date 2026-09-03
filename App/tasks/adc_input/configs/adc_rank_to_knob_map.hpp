#ifndef ADC_RANK_KNOB_CONFIG_TABLE_H
#define ADC_RANK_KNOB_CONFIG_TABLE_H

#include <cstdint>

#include "knob_id.h"
#include "stm32h7xx.h"

namespace AdcRankToKnobMap {

using AdcRank_t = std::uint8_t;
using AdcValue_t = std::uint16_t;
using AdcRankKnobConfigMapCallbackFunction = void(AdcRank_t adc_rank,
                                                  KnobId knob_id);

void Foreach(AdcRankKnobConfigMapCallbackFunction CallbackFunction);

}  // namespace AdcRankToKnobMap

#endif
