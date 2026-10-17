#ifndef FX_PARAMETER_STORE_HPP
#define FX_PARAMETER_STORE_HPP

#include <cstddef>

#include "cmsis_os2.h"
#include "enum_map.hpp"
#include "fx_entry.hpp"
#include "fx_type.hpp"
#include "fx_parameter_id.hpp"
#include "fx_section.hpp"
#include "fx_slot.hpp"
#include "fx_variant.hpp"
#include "parameter.hpp"
#include "singleton.hpp"

namespace Store {

class FxParameterStore : public Singleton<FxParameterStore> {
  friend class Singleton<FxParameterStore>;

 public:
  using FxParameterMap = EnumMap<Fx::ParameterId, ParameterModel::Parameter>;

  struct FxStore {
    FxParameterMap fx_map;
    Fx::Type selected_fx;
    bool is_on;
  };

 public:
  void Reset(osMutexId_t fx_parameter_mutex);

  void GetSelectedFxEntries(Fx::SlotEntries& ifx_entries,
                            Fx::SlotEntries& tfx_entries);

  void GetSelectedFxEntriesOrSkip(Fx::SlotEntries& ifx_entries,
                                  Fx::SlotEntries& tfx_entries);

  ParameterModel::Parameter& Get(Fx::Section section, Fx::Slot slot,
                                 Fx::ParameterId parameter_id);

 private:
  EnumMap<Fx::Section, EnumMap<Fx::Slot, FxStore>> stores;
  // EnumMap<Fx::Slot, FxStore> ifx_stores;
  // EnumMap<Fx::Slot, FxStore> tfx_stores;
  osMutexId_t mutex;
};

}  // namespace Store

#endif
