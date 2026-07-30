#include "adc_rank_knob_table.h"

#include "utils.h"

/**
 * 이 테이블의 길이는 hadc1.Init.NbrOfConversion과 같아야 한다.
 * ADC의 어느 채널에 어떤 노브가 매핑되어 있는지 소프트웨어적으로는 알 수 없으므로,
 *  여기서 하드코딩으로 정의한다.
 */ 
KnobId adc_rank_knob_table[] = {
    KNOB_ID_IFX,
    KNOB_ID_TFX,
    KNOB_ID_TRACK_1_VOLUME_FADER,
};
const size_t adc_rank_knob_table_count = ARRAY_COUNT(adc_rank_knob_table);
