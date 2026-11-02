#ifndef SYSTEM_PARAMETER_STORE_HPP
#define SYSTEM_PARAMETER_STORE_HPP

#include <functional>

#include "cmsis_os2.h"
#include "enum_map.hpp"
#include "mutex_guard.hpp"
#include "parameter.hpp"
#include "parameter_id.hpp"
#include "singleton.hpp"

/**
 * TODO:
 * 파라미터 수가 많지 않으므로 하나의 저장소에서 모든 파라미터를 관리한다.
 * 아직 다른 패널들이 개발되지 않았으므로 하나의 저장소로 충분하다.
 * 단, 파라미터 수가 50개 이상이 되는 경우 패널별로 분리해야한다.
 * 저장소를 분리한다면 ParameterId도 저장소에 따라 분리해야한다.
 */

namespace Store {

class SystemParameterStore : public Singleton<SystemParameterStore> {
 public:
  void Reset(osMutexId_t system_parameter_mutex);

  ParameterModel::Parameter& Get(ParameterModel::Id id);

  template <typename Operation>
  void WithParameterLocked(ParameterModel::Id id, Operation operation) {
    MutexGuard lock{mutex, 500UL};
    if (lock.is_acquired()) {
      operation(system_parameters.Get(id));
    }
  }

 private:
  EnumMap<ParameterModel::Id, ParameterModel::Parameter> system_parameters;
  osMutexId_t mutex;
};

}  // namespace Store

#endif
