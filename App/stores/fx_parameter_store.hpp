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
  struct FxEntry {
    Fx::Id id;
    Fx::Variant fx;
    bool is_on;
  };

  struct FxStore {
    EnumMap<Fx::Id, Fx::Variant> fx_map;
    Fx::Id selected_fx;
    bool is_on;
  };

 public:
  void Reset(osMutexId_t fx_parameter_mutex);

  using FxEntries = std::array<FxParameterStore::FxEntry,
                               static_cast<std::size_t>(Fx::Slot::COUNT)>;

  void GetSelectedFxEntries(FxParameterStore::FxEntries& ifx_entries,
                            FxParameterStore::FxEntries& tfx_entries);

  void GetSelectedFxEntriesOrSkip(FxParameterStore::FxEntries& ifx_entries,
                                  FxParameterStore::FxEntries& tfx_entries);

 private:
  EnumMap<Fx::Slot, FxStore> ifx_stores;
  EnumMap<Fx::Slot, FxStore> tfx_stores;
  osMutexId_t mutex;
};

}  // namespace Store

#endif
