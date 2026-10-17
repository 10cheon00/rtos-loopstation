#include "fx_parameter_store.hpp"

#include "enum_map.hpp"
#include "mutex_guard.hpp"

namespace Store {

using namespace ParameterModel;

namespace {

constexpr FxParameterStore::FxParameterMap INITIAL_FX_PARAMETERS{
    EnumEntry{Fx::ParameterId::FX_LPF_RATE,
              ValueVariant{UnsignedRangeValue{0, 100, 100}}},
    EnumEntry{Fx::ParameterId::FX_LPF_DEPTH,
              ValueVariant{UnsignedRangeValue{0, 100, 50}}},
    EnumEntry{Fx::ParameterId::FX_LPF_RESONANCE,
              ValueVariant{UnsignedRangeValue{0, 100, 50}}},
    EnumEntry{Fx::ParameterId::FX_LPF_CUTOFF,
              ValueVariant{UnsignedRangeValue{0, 100, 50}}},
};

static constexpr FxParameterStore::FxStore INITIAL_FX_STORE{
    .fx_map = INITIAL_FX_PARAMETERS,
    .selected_fx = Fx::Type::LPF,
    .is_on = false,
};

// 저장된 파라미터를 오디오 태스크에 전달할 DTO로 변환한다.
Fx::Variant GetSelectedFxVariant(FxParameterStore::FxStore& store) {
  switch (store.selected_fx) {
    case Fx::Type::LPF:
      return Fx::Parameters::Lpf{
          .rate = std::get<UnsignedRangeValue>(
              store.fx_map[Fx::ParameterId::FX_LPF_RATE].GetValueVariant()),
          .depth = std::get<UnsignedRangeValue>(
              store.fx_map[Fx::ParameterId::FX_LPF_DEPTH].GetValueVariant()),
          .resonance = std::get<UnsignedRangeValue>(
              store.fx_map[Fx::ParameterId::FX_LPF_RESONANCE]
                  .GetValueVariant()),
          .cutoff = std::get<UnsignedRangeValue>(
              store.fx_map[Fx::ParameterId::FX_LPF_CUTOFF].GetValueVariant()),
      };
    default:
      return std::monostate{};
  }
}

}  // namespace

void FxParameterStore::Reset(osMutexId_t mutex) {
  this->stores[Fx::Section::IFX] = EnumMap<Fx::Slot, FxStore>{
      EnumEntry{Fx::Slot::A, INITIAL_FX_STORE},
      EnumEntry{Fx::Slot::B, INITIAL_FX_STORE},
      EnumEntry{Fx::Slot::C, INITIAL_FX_STORE},
  };
  this->stores[Fx::Section::TFX] = EnumMap<Fx::Slot, FxStore>{
      EnumEntry{Fx::Slot::A, INITIAL_FX_STORE},
      EnumEntry{Fx::Slot::B, INITIAL_FX_STORE},
      EnumEntry{Fx::Slot::C, INITIAL_FX_STORE},
  };
  this->mutex = mutex;
}

void FxParameterStore::GetSelectedFxEntries(Fx::SlotEntries& ifx_entries,
                                            Fx::SlotEntries& tfx_entries) {
  MutexGuard lock{this->mutex, 500UL};

  using Underlying = std::underlying_type_t<Fx::Slot>;

  for (Underlying i = 0; i < static_cast<Underlying>(Fx::Slot::COUNT); ++i) {
    const auto fx_slot = static_cast<Fx::Slot>(i);
    Fx::Type selected_fx = this->stores[Fx::Section::IFX][fx_slot].selected_fx;
    ifx_entries[i].type = selected_fx;
    ifx_entries[i].fx =
        GetSelectedFxVariant(this->stores[Fx::Section::IFX][fx_slot]);
    ifx_entries[i].is_on = this->stores[Fx::Section::IFX][fx_slot].is_on;

    selected_fx = this->stores[Fx::Section::TFX][fx_slot].selected_fx;
    tfx_entries[i].type = selected_fx;
    tfx_entries[i].fx =
        GetSelectedFxVariant(this->stores[Fx::Section::TFX][fx_slot]);
    tfx_entries[i].is_on = this->stores[Fx::Section::TFX][fx_slot].is_on;
  }
}

void FxParameterStore::GetSelectedFxEntriesOrSkip(
    Fx::SlotEntries& ifx_entries, Fx::SlotEntries& tfx_entries) {
  MutexGuard lock{this->mutex, 0UL};
  if (!lock.is_acquired()) {
    for (auto& fx_entry : ifx_entries) {
      fx_entry.type = Fx::Type::NONE;
      fx_entry.fx = std::monostate{};
      fx_entry.is_on = false;
    }
    for (auto& fx_entry : tfx_entries) {
      fx_entry.type = Fx::Type::NONE;
      fx_entry.fx = std::monostate{};
      fx_entry.is_on = false;
    }
    return;
  }

  using Underlying = std::underlying_type_t<Fx::Slot>;

  for (Underlying i = 0; i < static_cast<Underlying>(Fx::Slot::COUNT); ++i) {
    const auto fx_slot = static_cast<Fx::Slot>(i);
    Fx::Type selected_fx = this->stores[Fx::Section::IFX][fx_slot].selected_fx;
    ifx_entries[i].type = selected_fx;
    ifx_entries[i].fx =
        GetSelectedFxVariant(this->stores[Fx::Section::IFX][fx_slot]);
    ifx_entries[i].is_on = this->stores[Fx::Section::IFX][fx_slot].is_on;

    selected_fx = this->stores[Fx::Section::TFX][fx_slot].selected_fx;
    tfx_entries[i].type = selected_fx;
    tfx_entries[i].fx =
        GetSelectedFxVariant(this->stores[Fx::Section::TFX][fx_slot]);
    tfx_entries[i].is_on = this->stores[Fx::Section::TFX][fx_slot].is_on;
  }
}

ParameterModel::Parameter& FxParameterStore::Get(Fx::Section section,
                                                 Fx::Slot slot,
                                                 Fx::ParameterId parameter_id) {
  MutexGuard lock{this->mutex, 500UL};
  if (lock.is_acquired()) {
    // TODO: ifx인지 tfx인지 결정하는 인자 추가해야함
    return this->stores[section][slot].fx_map.Get(parameter_id);
  }
  return this->stores[section][slot].fx_map.Get(Fx::ParameterId::NONE);
}

}  // namespace Store
