#ifndef ADC_RANK_TO_KNOB_MAP_H
#define ADC_RANK_TO_KNOB_MAP_H

#include <cstdint>

#include "knob_id.hpp"
#include "stm32h7xx.h"

namespace AdcRankToKnobMap {

using AdcRank_t = std::uint8_t;
using AdcValue_t = std::uint16_t;
using AdcRankKnobConfigMapCallbackFunction = void(AdcRank_t adc_rank,
                                                  KnobId knob_id);

void Foreach(AdcRankKnobConfigMapCallbackFunction CallbackFunction);

}  // namespace AdcRankToKnobMap

#endif
