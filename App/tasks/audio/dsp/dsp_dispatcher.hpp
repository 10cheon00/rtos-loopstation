#ifndef DSP_DISPATCHER_HPP
#define DSP_DISPATCHER_HPP

#include <cstdint>

#include "audio_config.h"
#include "dsp_fx_lpf.hpp"
#include "fx_entry.hpp"
#include "fx_parameter_store.hpp"

namespace DSP {

class Dispatcher {
 public:
  void Dispatch(Fx::Entry& fx_entry, BitDepth_t* buffer, uint32_t size);

 private:
  DSP::FxProcessor::Lpf lpf;
};

}  // namespace DSP

#endif
