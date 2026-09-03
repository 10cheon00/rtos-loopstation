#ifndef LOOPSTATION_PARAMETER_STORE_HPP
#define LOOPSTATION_PARAMETER_STORE_HPP

#include "enum_map.hpp"
#include "parameter.h"
#include "parameter_id.hpp"

/**
 * TODO:
 * 파라미터 수가 많지 않으므로 하나의 저장소에서 모든 파라미터를 관리한다.
 * 아직 다른 패널들이 개발되지 않았으므로 하나의 저장소로 충분하다.
 * 단, 파라미터 수가 50개 이상이 되는 경우 패널별로 분리해야한다.
 * 저장소를 분리한다면 ParameterId도 저장소에 따라 분리해야한다.
 */

namespace LoopstationStore {

using ParameterStore = EnumMap<ParameterId, Parameter>;

Parameter& GetParameter(ParameterId parameter_id);

}  // namespace LoopstationStore

#endif
