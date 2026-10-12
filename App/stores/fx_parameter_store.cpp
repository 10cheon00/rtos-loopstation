#include "fx_parameter_store.hpp"

#include "enum_map.hpp"

namespace Store {

using namespace ParameterModel;

using FxStore = EnumMap<FxId, Parameter>;

static constexpr FxStore initial_fx_store{
    EnumEntry{
        FxId::FX_LPF_RATE,
        Parameter{UnsignedRangeValue(
            0, 100, 100)},  // TODO: 음표와 숫자가 혼합된 파라미터 구현해야함
    },
    EnumEntry{
        FxId::FX_LPF_DEPTH,
        Parameter{UnsignedRangeValue(0, 100, 50)},
    },
    EnumEntry{
        FxId::FX_LPF_RESONANCE,
        Parameter{UnsignedRangeValue(0, 100, 50)},
    },
    EnumEntry{
        FxId::FX_LPF_CUTOFF,
        Parameter{UnsignedRangeValue(0, 100, 50)},
    },

    EnumEntry{
        FxId::FX_HPF_RATE,
        Parameter{UnsignedRangeValue(
            0, 100, 100)},  // TODO: 음표와 숫자가 혼합된 파라미터 구현해야함
    },
    EnumEntry{
        FxId::FX_HPF_DEPTH,
        Parameter{UnsignedRangeValue(0, 100, 50)},
    },
    EnumEntry{
        FxId::FX_HPF_RESONANCE,
        Parameter{UnsignedRangeValue(0, 100, 50)},
    },
    EnumEntry{
        FxId::FX_HPF_CUTOFF,
        Parameter{UnsignedRangeValue(0, 100, 50)},
    },
};

void FxParameterStore::Reset(osMutexId_t mutex) {
  fx_stores = EnumMap<FxSlot, FxStore>{
      EnumEntry{FxSlot::A, initial_fx_store},
      EnumEntry{FxSlot::B, initial_fx_store},
      EnumEntry{FxSlot::C, initial_fx_store},
  };
  this->mutex = mutex;
}

ParameterModel::Parameter& FxParameterStore::Get(FxSlot slot, FxId id) {
  return fx_stores.Get(slot).Get(id);
}

}  // namespace Store
