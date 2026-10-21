#ifndef FX_PARAMETER_ID_HPP
#define FX_PARAMETER_ID_HPP

namespace Fx {

enum class ParameterId {
  NONE,
  FX_SYSTEM_SELECTED_FX,
  FX_SYSTEM_FX_STATE,
  FX_LPF_RATE,
  FX_LPF_DEPTH,
  FX_LPF_RESONANCE,
  FX_LPF_CUTOFF,
  COUNT,
};

}

#endif