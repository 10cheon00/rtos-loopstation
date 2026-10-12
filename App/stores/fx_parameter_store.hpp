#ifndef FX_PARAMETER_STORE_HPP
#define FX_PARAMETER_STORE_HPP

#include "cmsis_os2.h"
#include "enum_map.hpp"
#include "fx_id.hpp"
#include "fx_slot.hpp"
#include "fx_variant.hpp"
#include "parameter.hpp"
#include "singleton.hpp"

namespace Store {

class FxParameterStore : public Singleton<FxParameterStore> {
  friend class Singleton<FxParameterStore>;

 public:
  void Reset(osMutexId_t fx_parameter_mutex);
  Fx::Variant& Get(Fx::Slot slot, Fx::Id id);

 private:
  using FxStore = EnumMap<Fx::Id, Fx::Variant>;
  EnumMap<Fx::Slot, FxStore> fx_stores;
  osMutexId_t mutex;
};

}  // namespace Store

#endif
