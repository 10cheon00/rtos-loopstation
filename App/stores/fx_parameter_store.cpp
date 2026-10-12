#include "fx_parameter_store.hpp"

#include "enum_map.hpp"
#include "mutex_guard.hpp"

namespace Store {

using namespace ParameterModel;

using FxStore = EnumMap<Fx::Id, Fx::Variant>;

static constexpr FxStore initial_fx_store{
    EnumEntry{Fx::Id::LPF, Fx::Variant{Fx::Lpf{}}},
};

void FxParameterStore::Reset(osMutexId_t mutex) {
  fx_stores = EnumMap<Fx::Slot, FxStore>{
      EnumEntry{Fx::Slot::A, initial_fx_store},
      EnumEntry{Fx::Slot::B, initial_fx_store},
      EnumEntry{Fx::Slot::C, initial_fx_store},
  };
  this->mutex = mutex;
}

Fx::Variant& FxParameterStore::Get(Fx::Slot slot, Fx::Id id) {
  MutexGuard lock{mutex, 500UL};
  return fx_stores.Get(slot).Get(id);
}

}  // namespace Store
