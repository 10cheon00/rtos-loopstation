#include "dsp_fx_lpf.hpp"

#include <cmath>

#include "audio_config.h"
#include "dsp_utils.hpp"
#include "utils.hpp"

namespace DSP::FxProcessor {

constexpr float q_min = 0.70710678f;
constexpr float q_max = 10.0f;  // 원하는 최대 공진 세기로 조정

void Lpf::Process(Fx::Parameters::Lpf& lpf_parameters, bool is_on,
                  BitDepth_t* buffer, std::size_t size) {
  // 0. 현재 선택한 Fx를 적용할지 판단.
  // 만약 적용하지 않는다면 LPF의 경우 처리를 하지 않지만, Delay나 Reverb같은
  // Fx들은 기존에 처리한 잔향 소리를 계속 처리할 수 있다.
  if (!is_on) {
    return;
  }

  // 1. 파라미터를 수식에 적용가능한 형태로 변환
  // TODO: Rate와 Depth는 LFO를 구현한 후 적용하기
  float rate = SAMPLING_FREQUENCY;
  float cutoff = Dsp::Utils::GetCutoffFrequency(lpf_parameters.cutoff);
  // float q = 1;  // TODO: 파라미터의 resonance를 변환하여 사용하기
  const float normalized = (float)lpf_parameters.resonance.current / 100.0f;
  const float q = q_min * std::pow(q_max / q_min, normalized);

  // 2. 수식에 필요한 변수를 업데이트
  float omega_degree = 360.0f * cutoff / rate;
  float cos = Utils::CosineFromDegrees(omega_degree);
  float sin = Utils::SineFromDegrees(omega_degree);
  float alpha = sin / (2.0f * q);

  float a0 = 1.0f + alpha;
  b0 = ((1.0f - cos) * 0.5f) / a0;
  b1 = (1.0f - cos) / a0;
  b2 = ((1.0f - cos) * 0.5f) / a0;
  a1 = (-2.0f * cos) / a0;
  a2 = (1.0f - alpha) / a0;

  // 3. 버퍼에 필터 적용
  float x = 0, y = 0;
  for (std::size_t i = 0; i < size; i += FRAME_BUFFER_COUNT) {
    x = static_cast<float>(buffer[i + INMP441_ALIGN_OFFSET]);
    y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    buffer[i + INMP441_ALIGN_OFFSET] = static_cast<BitDepth_t>(y);
  }
}

}  // namespace DSP::FxProcessor
