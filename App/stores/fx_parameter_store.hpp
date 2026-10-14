#ifndef FX_PARAMETER_STORE_HPP
#define FX_PARAMETER_STORE_HPP

#include "cmsis_os2.h"
#include "enum_map.hpp"
#include "fx_entry.hpp"
#include "fx_id.hpp"
#include "fx_slot.hpp"
#include "fx_variant.hpp"
#include "parameter.hpp"
#include "singleton.hpp"

namespace Store {

class FxParameterStore : public Singleton<FxParameterStore> {
  friend class Singleton<FxParameterStore>;

 public:
  struct FxStore {
    EnumMap<Fx::Id, Fx::Variant> fx_map;
    Fx::Id selected_fx;
    bool is_on;
  };

 public:
  void Reset(osMutexId_t fx_parameter_mutex);

  void GetSelectedFxEntries(Fx::SlotEntries& ifx_entries,
                            Fx::SlotEntries& tfx_entries);

  void GetSelectedFxEntriesOrSkip(Fx::SlotEntries& ifx_entries,
                                  Fx::SlotEntries& tfx_entries);

 private:
  EnumMap<Fx::Slot, FxStore> ifx_stores;
  EnumMap<Fx::Slot, FxStore> tfx_stores;
  osMutexId_t mutex;
};

}  // namespace Store

#endif
