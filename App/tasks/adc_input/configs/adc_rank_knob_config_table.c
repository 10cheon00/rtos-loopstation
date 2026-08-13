#include "adc_rank_knob_config_table.h"

#include "utils.h"
#include "config_table.h"
#include "config_validator.h"

/**
 * 이 테이블의 길이는 hadc1.Init.NbrOfConversion과 같아야 한다.
 * ADC의 어느 채널에 어떤 노브가 매핑되어 있는지 소프트웨어적으로는 알 수 없으므로,
 *  여기서 하드코딩으로 정의한다.
 */
#define ENTRIES                                                                                    \
    ConfigTable_1D_ENTRY(0, KNOB_ID_IFX), ConfigTable_1D_ENTRY(1, KNOB_ID_TFX),                    \
        ConfigTable_1D_ENTRY(2, KNOB_ID_TRACK_1_VOLUME_FADER)

ConfigTable_1D_DECLARE_TABLE(AdcRank_t, KnobId, 3, ENTRIES);
#undef ENTRIES

ConfigValidator_REGISTER_CONFIG_TABLE_1D(AdcRank_t, KnobId, 3, KNOB_ID_NONE, KNOB_ID_COUNT,
                                         CONFIG_TABLE_TYPE_NO_NULL_VALUE);

void AdcRankKnobConfigTable_Foreach(AdcRankKnobConfigMapCallbackFunction CallbackFunction)
{
    for (AdcRank_t i = 0; i < KNOB_ID_COUNT; i++) {
        CallbackFunction((AdcRank_t)i, ConfigTable_NAME(AdcRank_t, KnobId)[i]);
    }
}
