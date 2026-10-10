#ifndef FX_PARAMETER_STORE
#define FX_PARAMETER_STORE

#include "fx_id.hpp"
#include "parameter.hpp"

namespace LoopstationStore::Fx {

enum class FxSlot {
  A,
  B,
  C,
  COUNT,
};

ParameterModel::Parameter& Get(FxSlot fx_slot, FxId id);

}  // namespace LoopstationStore::Fx

#endif
