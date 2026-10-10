#include "fx_parameter_store.hpp"

#include "enum_map.hpp"

namespace LoopstationStore::Fx {

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

static EnumMap<FxSlot, FxStore> fx_stores{
    EnumEntry{FxSlot::A, initial_fx_store},
    EnumEntry{FxSlot::B, initial_fx_store},
    EnumEntry{FxSlot::C, initial_fx_store},
};

ParameterModel::Parameter& Get(FxSlot fx_slot, FxId id) {
  return fx_stores.Get(fx_slot).Get(id);
}

}  // namespace LoopstationStore::Fx
