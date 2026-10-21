#ifndef FX_PARAMETER_STORE_HPP
#define FX_PARAMETER_STORE_HPP

#include <cstddef>

#include "cmsis_os2.h"
#include "enum_map.hpp"
#include "fx_entry.hpp"
#include "fx_parameter_id.hpp"
#include "fx_section.hpp"
#include "fx_slot.hpp"
#include "fx_type.hpp"
#include "fx_variant.hpp"
#include "mutex_guard.hpp"
#include "parameter.hpp"
#include "singleton.hpp"

namespace Store {

class FxParameterStore : public Singleton<FxParameterStore> {
  friend class Singleton<FxParameterStore>;

 public:
  using FxParameterMap = EnumMap<Fx::ParameterId, ParameterModel::Parameter>;

 public:
  void Reset(osMutexId_t fx_parameter_mutex);

  void GetSelectedFxEntries(Fx::SlotEntries& ifx_entries,
                            Fx::SlotEntries& tfx_entries);

  void GetSelectedFxEntriesOrSkip(Fx::SlotEntries& ifx_entries,
                                  Fx::SlotEntries& tfx_entries);

  const ParameterModel::Parameter& Get(Fx::Section section, Fx::Slot slot,
                                       Fx::ParameterId parameter_id);
  template <typename Operation>
  void WithParameterLocked(Fx::Section section, Fx::Slot slot,
                           Fx::ParameterId parameter_id, Operation operation) {
    MutexGuard lock{this->mutex, 500UL};
    if (lock.is_acquired()) {
      operation(this->stores[section][slot][parameter_id]);
    }
  }

 private:
  EnumMap<Fx::Section, EnumMap<Fx::Slot, FxParameterMap>> stores;
  osMutexId_t mutex;

  void CopyToFxEntry(Fx::Section section, Fx::Slot slot, Fx::Entry& entry);
  Fx::Type GetSelectedFxType(Fx::Section section, Fx::Slot slot);
  bool GetFxState(Fx::Section section, Fx::Slot slot);
};

}  // namespace Store

#endif
