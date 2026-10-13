#include "fx_parameter_store.hpp"

#include "enum_map.hpp"
#include "mutex_guard.hpp"

namespace Store {

using namespace ParameterModel;

static constexpr EnumMap<Fx::Id, Fx::Variant> INITIAL_FX_PARAMETERS{
    EnumEntry{Fx::Id::LPF, Fx::Variant{Fx::Lpf{}}}};

static constexpr FxParameterStore::FxStore INITIAL_FX_STORE{
    .fx_map = INITIAL_FX_PARAMETERS,
    .selected_fx = Fx::Id::LPF,
    .status = false,
};

void FxParameterStore::Reset(osMutexId_t mutex) {
  ifx_stores = EnumMap<Fx::Slot, FxStore>{
      EnumEntry{Fx::Slot::A, INITIAL_FX_STORE},
      EnumEntry{Fx::Slot::B, INITIAL_FX_STORE},
      EnumEntry{Fx::Slot::C, INITIAL_FX_STORE},
  };
  tfx_stores = EnumMap<Fx::Slot, FxStore>{
      EnumEntry{Fx::Slot::A, INITIAL_FX_STORE},
      EnumEntry{Fx::Slot::B, INITIAL_FX_STORE},
      EnumEntry{Fx::Slot::C, INITIAL_FX_STORE},
  };
  this->mutex = mutex;
}

void FxParameterStore::GetSelectedFxEntries(
    FxParameterStore::FxEntries& ifx_entries,
    FxParameterStore::FxEntries& tfx_entries) {
  MutexGuard lock{this->mutex, 500UL};

  using Underlying = std::underlying_type_t<Fx::Slot>;

  for (Underlying i = 0; i < static_cast<Underlying>(Fx::Slot::COUNT); ++i) {
    const auto fx_slot = static_cast<Fx::Slot>(i);
    Fx::Id selected_fx = this->ifx_stores[fx_slot].selected_fx;
    ifx_entries[i].id = selected_fx;
    ifx_entries[i].fx = this->ifx_stores[fx_slot].fx_map[selected_fx];
    ifx_entries[i].status = this->ifx_stores[fx_slot].status;

    selected_fx = this->tfx_stores[fx_slot].selected_fx;
    tfx_entries[i].id = selected_fx;
    tfx_entries[i].fx = this->tfx_stores[fx_slot].fx_map[selected_fx];
    tfx_entries[i].status = this->tfx_stores[fx_slot].status;
  }
}

void FxParameterStore::GetSelectedFxEntriesOrSkip(
    FxParameterStore::FxEntries& ifx_entries,
    FxParameterStore::FxEntries& tfx_entries) {
  MutexGuard lock{this->mutex, 0UL};
  if (!lock.is_acquired()) {
    for (auto& fx_entry : ifx_entries) {
      fx_entry.id = Fx::Id::NONE;
      fx_entry.fx = std::monostate{};
      fx_entry.status = false;
    }
    for (auto& fx_entry : tfx_entries) {
      fx_entry.id = Fx::Id::NONE;
      fx_entry.fx = std::monostate{};
      fx_entry.status = false;
    }
    return;
  }

  using Underlying = std::underlying_type_t<Fx::Slot>;

  for (Underlying i = 0; i < static_cast<Underlying>(Fx::Slot::COUNT); ++i) {
    const auto fx_slot = static_cast<Fx::Slot>(i);
    Fx::Id selected_fx = this->ifx_stores[fx_slot].selected_fx;
    ifx_entries[i].id = selected_fx;
    ifx_entries[i].fx = this->ifx_stores[fx_slot].fx_map[selected_fx];
    ifx_entries[i].status = this->ifx_stores[fx_slot].status;

    selected_fx = this->tfx_stores[fx_slot].selected_fx;
    tfx_entries[i].id = selected_fx;
    tfx_entries[i].fx = this->tfx_stores[fx_slot].fx_map[selected_fx];
    tfx_entries[i].status = this->tfx_stores[fx_slot].status;
  }
}

}  // namespace Store
