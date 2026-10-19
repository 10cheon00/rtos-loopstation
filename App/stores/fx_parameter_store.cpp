#include "fx_parameter_store.hpp"

#include "enum_map.hpp"
#include "mutex_guard.hpp"

namespace Store {

using namespace ParameterModel;

namespace {

constexpr FxParameterStore::FxParameterMap INITIAL_FX_PARAMETERS{
    EnumEntry{Fx::ParameterId::FX_SYSTEM_SELECTED_FX,
              ValueVariant(FxSelectedFxEnumValue{Fx::Type::LPF})},
    EnumEntry{Fx::ParameterId::FX_SYSTEM_FX_STATE,
              ValueVariant(ToggleValue{false})},
    EnumEntry{Fx::ParameterId::FX_LPF_RATE,
              ValueVariant{UnsignedRangeValue{0, 100, 100}}},
    EnumEntry{Fx::ParameterId::FX_LPF_DEPTH,
              ValueVariant{UnsignedRangeValue{0, 100, 50}}},
    EnumEntry{Fx::ParameterId::FX_LPF_RESONANCE,
              ValueVariant{UnsignedRangeValue{0, 100, 50}}},
    EnumEntry{Fx::ParameterId::FX_LPF_CUTOFF,
              ValueVariant{UnsignedRangeValue{0, 100, 50}}},
};

// 저장된 파라미터를 오디오 태스크에 전달할 DTO로 변환한다.
Fx::Variant GetSelectedFxVariant(FxParameterStore::FxParameterMap& fx_map) {
  Fx::Type selected_fx = static_cast<Fx::Type>(
      std::get<FxSelectedFxEnumValue>(
          fx_map[Fx::ParameterId::FX_SYSTEM_SELECTED_FX].GetValueVariant())
          .value);

  switch (selected_fx) {
    case Fx::Type::LPF:
      return Fx::Parameters::Lpf{
          .rate = std::get<UnsignedRangeValue>(
              fx_map[Fx::ParameterId::FX_LPF_RATE].GetValueVariant()),
          .depth = std::get<UnsignedRangeValue>(
              fx_map[Fx::ParameterId::FX_LPF_DEPTH].GetValueVariant()),
          .resonance = std::get<UnsignedRangeValue>(
              fx_map[Fx::ParameterId::FX_LPF_RESONANCE].GetValueVariant()),
          .cutoff = std::get<UnsignedRangeValue>(
              fx_map[Fx::ParameterId::FX_LPF_CUTOFF].GetValueVariant()),
      };
    default:
      return std::monostate{};
  }
}

}  // namespace

void FxParameterStore::Reset(osMutexId_t mutex) {
  stores[Fx::Section::IFX][Fx::Slot::A] = INITIAL_FX_PARAMETERS;
  stores[Fx::Section::IFX][Fx::Slot::B] = INITIAL_FX_PARAMETERS;
  stores[Fx::Section::IFX][Fx::Slot::C] = INITIAL_FX_PARAMETERS;
  stores[Fx::Section::TFX][Fx::Slot::A] = INITIAL_FX_PARAMETERS;
  stores[Fx::Section::TFX][Fx::Slot::B] = INITIAL_FX_PARAMETERS;
  stores[Fx::Section::TFX][Fx::Slot::C] = INITIAL_FX_PARAMETERS;
  this->mutex = mutex;
}

void FxParameterStore::GetSelectedFxEntries(Fx::SlotEntries& ifx_entries,
                                            Fx::SlotEntries& tfx_entries) {
  MutexGuard lock{this->mutex, 500UL};

  if (lock.is_acquired()) {
    using Underlying = std::underlying_type_t<Fx::Slot>;

    for (Underlying i = 0; i < static_cast<Underlying>(Fx::Slot::COUNT); ++i) {
      const auto slot = static_cast<Fx::Slot>(i);

      CopyToFxEntry(Fx::Section::IFX, slot, ifx_entries[i]);
      CopyToFxEntry(Fx::Section::TFX, slot, tfx_entries[i]);
    }
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
    const auto slot = static_cast<Fx::Slot>(i);
    CopyToFxEntry(Fx::Section::IFX, slot, ifx_entries[i]);
    CopyToFxEntry(Fx::Section::TFX, slot, tfx_entries[i]);
  }
}

const ParameterModel::Parameter& FxParameterStore::Get(
    Fx::Section section, Fx::Slot slot, Fx::ParameterId parameter_id) {
  MutexGuard lock{this->mutex, 500UL};
  if (lock.is_acquired()) {
    return this->stores[section][slot][parameter_id];
  }
  return this->stores[section][slot][Fx::ParameterId::NONE];
}

Fx::Type FxParameterStore::GetSelectedFxType(Fx::Section section,
                                             Fx::Slot slot) {
  return static_cast<Fx::Type>(
      std::get<FxSelectedFxEnumValue>(
          stores[section][slot][Fx::ParameterId::FX_SYSTEM_SELECTED_FX]
              .GetValueVariant())
          .value);
}

bool FxParameterStore::GetFxState(Fx::Section section, Fx::Slot slot) {
  return std::get<ToggleValue>(
             stores[section][slot][Fx::ParameterId::FX_SYSTEM_FX_STATE]
                 .GetValueVariant())
      .is_on;
}

void FxParameterStore::CopyToFxEntry(Fx::Section section, Fx::Slot slot,
                                     Fx::Entry& entry) {
  Fx::Type selected_fx = GetSelectedFxType(section, slot);
  bool is_on = GetFxState(section, slot);

  entry.type = selected_fx;
  entry.is_on = is_on;
  entry.fx = GetSelectedFxVariant(stores[section][slot]);
}

}  // namespace Store
