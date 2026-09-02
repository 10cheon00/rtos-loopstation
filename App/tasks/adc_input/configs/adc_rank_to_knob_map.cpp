#include "adc_rank_to_knob_map.hpp"

#include "enum_map.hpp"

namespace AdcRankToKnobMap {
/**
 * 이 테이블의 길이는 hadc1.Init.NbrOfConversion과 같아야 한다.
 * ADC의 어느 채널에 어떤 노브가 매핑되어 있는지 소프트웨어적으로는 알 수
 * 없으므로, 여기서 하드코딩으로 정의한다.
 */
static constexpr EnumMap<AdcRank_t, KnobId, 3> adc_rank_knob_map{
    EnumEntry{static_cast<AdcRank_t>(0), KnobId::IFX},
    EnumEntry{static_cast<AdcRank_t>(1), KnobId::TFX},
    EnumEntry{static_cast<AdcRank_t>(2), KnobId::TRACK_1_VOLUME_FADER},
};

void Foreach(AdcRankKnobConfigMapCallbackFunction CallbackFunction) {
  // TODO:
  // 하드웨어적으로 등록된 adc 채널에 대해서만 수행하도록 임시 수정
  for (AdcRank_t i = 0; i < 5; i++) {
    if (i == ID_NONE || i == ID_NULL) {
      continue;
    }
    CallbackFunction(i, adc_rank_knob_map[i]);
  }
}

}  // namespace AdcRankToKnobMap
