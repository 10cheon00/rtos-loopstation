#ifndef DSP_FX_LPF_HPP
#define DSP_FX_LPF_HPP

#include "audio_config.hpp"
#include "fx_variant.hpp"

namespace DSP::FxProcessor {

class Lpf {
 public:
  Lpf() { Reset(); }

  void Reset() {
    a1 = a2 = b0 = b1 = b2 = 0;
    x1 = x2 = y1 = y2 = 0;
  }

  void Process(Fx::Parameters::Lpf& lpf_parameters, bool is_on,
               BitDepth_t* buffer, std::size_t size);

 private:
  float a1, a2, b0, b1, b2;
  float x1, x2, y1, y2;
};

};  // namespace DSP::FxProcessor

#endif
