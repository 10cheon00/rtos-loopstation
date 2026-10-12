#ifndef FX_PARAMETER_STORE_HPP
#define FX_PARAMETER_STORE_HPP

#include "enum_map.hpp"
#include "parameter.hpp"
#include "parameter_id.hpp"
#include "singleton.hpp"

namespace Store {

class FxParameterStore : public Singleton<FxParameterStore> {
 public:
  void Reset();

  ParameterModel::Parameter& Get(ParameterModel::Id id);

 private:
  EnumMap<ParameterModel::Id, ParameterModel::Parameter> fx_parameters;
};

}  // namespace Store

#endif
