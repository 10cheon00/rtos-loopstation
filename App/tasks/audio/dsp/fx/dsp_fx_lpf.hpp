#ifndef DSP_FX_LPF_HPP
#define DSP_FX_LPF_HPP

#include "dsp_processable.hpp"

namespace DSP::FxProcessor {

class Lpf : public Processable {
 public:
  virtual void Process();
};

};  // namespace DSP::FxProcessor

#endif
