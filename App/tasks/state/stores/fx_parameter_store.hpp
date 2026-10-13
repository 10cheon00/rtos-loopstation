#ifndef FX_PARAMETER_STORE
#define FX_PARAMETER_STORE

#include "fx_id.hpp"
#include "fx_slot.hpp"
#include "parameter.hpp"

namespace LoopstationStore::Fx {

ParameterModel::Parameter& Get(FxSlot fx_slot, FxId id);

}  // namespace LoopstationStore::Fx

#endif
