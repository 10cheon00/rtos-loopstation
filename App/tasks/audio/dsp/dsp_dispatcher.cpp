#include "dsp_dispatcher.hpp"

#include <variant>

namespace DSP {

void Dispatcher::Dispatch(Fx::Entry& fx_entry, BitDepth_t* buffer,
                          uint32_t size) {
  std::visit(
      [this, is_on = fx_entry.is_on, buffer, size](auto& parameters) {
        using T = std::decay_t<decltype(parameters)>;

        if constexpr (std::is_same_v<T, Fx::Parameters::Lpf>) {
          // TODO:
          // 필요한 정보를 넘겨줄 수 있도록 수정
          lpf.Process(parameters, is_on, buffer, size);
        } else {
          // NOP
        }
      },
      fx_entry.fx);
}

}  // namespace DSP