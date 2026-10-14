#ifndef DSP_FX_LPF_HPP
#define DSP_FX_LPF_HPP

#include "audio_config.h"
#include "fx_variant.hpp"

namespace DSP::FxProcessor {

class Lpf {
 public:
  void Process(Fx::Parameters::Lpf& lpf_parameters, BitDepth_t* buffer,
               uint32_t size);
};

};  // namespace DSP::FxProcessor

#endif
