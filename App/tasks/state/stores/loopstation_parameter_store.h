#ifndef LOOPSTATION_PARAMETER_STORE_H
#define LOOPSTATION_PARAMETER_STORE_H

#include "panel_parameter_table.h"

/**
 * TODO:
 * 파라미터 수가 많지 않으므로 하나의 저장소에서 모든 파라미터를 관리한다.
 * 아직 다른 패널들이 개발되지 않았으므로 하나의 저장소로 충분하다.
 * 단, 파라미터 수가 50개 이상이 되는 경우 패널별로 분리해야한다.
 * 저장소를 분리한다면 ParameterId도 저장소에 따라 분리해야한다.
 */
typedef struct {
    Parameter parameters[PARAMETER_ID_COUNT];
} LoopStationParameterStore;

Parameter *LoopStationParameterStore_GetParameterFromParameterId(ParameterId parameter_id);

#endif
