#include "adc_rank_knob_config_map.h"

#include "utils.h"
#include "config_map.h"
#include "config_validator.h"

/**
 * 이 테이블의 길이는 hadc1.Init.NbrOfConversion과 같아야 한다.
 * ADC의 어느 채널에 어떤 노브가 매핑되어 있는지 소프트웨어적으로는 알 수 없으므로,
 *  여기서 하드코딩으로 정의한다.
 */
static ConfigMapEntry adc_rank_knob_config_map_entries[] = {
    {.key = 0, .value = KNOB_ID_IFX},
    {.key = 1, .value = KNOB_ID_TFX},
    {.key = 2, .value = KNOB_ID_TRACK_1_VOLUME_FADER},
};

static ConfigMap adc_rank_knob_config_map = {.entries = adc_rank_knob_config_map_entries,
                                             .count =
                                                 ARRAY_COUNT(adc_rank_knob_config_map_entries)};

ConfigValidator_REGISTER(&adc_rank_knob_config_map, AdcRank_t, KnobId);

void AdcRankKnobConfigMap_Foreach(AdcRankKnobConfigMapCallbackFunction CallbackFunction)
{
    for (AdcRank_t i = 0; i < adc_rank_knob_config_map.count; i++) {
        CallbackFunction(adc_rank_knob_config_map.entries[i].key,
                         adc_rank_knob_config_map.entries[i].value);
    }
}
