#ifndef FX_PARAMETER_STORE_HPP
#define FX_PARAMETER_STORE_HPP

#include "enum_map.hpp"
#include "fx_id.hpp"
#include "fx_slot.hpp"
#include "parameter.hpp"
#include "singleton.hpp"

namespace Store {

class FxParameterStore : public Singleton<FxParameterStore> {
  friend class Singleton<FxParameterStore>;

 public:
  void Reset();
  ParameterModel::Parameter& Get(FxSlot slot, FxId id);

 private:
  FxParameterStore() { Reset(); }
  ~FxParameterStore() = default;

  using FxStore = EnumMap<FxId, ParameterModel::Parameter>;
  EnumMap<FxSlot, FxStore> fx_stores;
};

}  // namespace Store

#endif
