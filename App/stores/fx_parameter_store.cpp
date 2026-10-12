#include "fx_parameter_store.hpp"

#include <cstddef>

#include "parameter_value.hpp"

namespace Store {

using namespace ParameterModel;

void FxParameterStore::Reset() {
    // TODO:
    // 초기화
}

ParameterModel::Parameter& FxParameterStore::Get(ParameterModel::Id id) {
  return fx_parameters.Get(id);
}

}  // namespace Store
