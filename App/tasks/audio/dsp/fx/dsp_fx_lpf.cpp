#include "dsp_fx_lpf.hpp"

#include <cmath>

#include "audio_config.hpp"
#include "dsp_utils.hpp"
#include "utils.hpp"

namespace DSP::FxProcessor {

void Lpf::Process(Fx::Parameters::Lpf& lpf_parameters, bool is_on,
                  BitDepth_t* buffer, std::size_t size) {
  // 0. 현재 선택한 Fx를 적용할지 판단.
  // 만약 적용하지 않는다면 LPF의 경우 처리를 하지 않지만, Delay나 Reverb같은
  // Fx들은 기존에 처리한 잔향 소리를 계속 처리할 수 있다.
  if (!is_on) {
    return;
  }

  // 1. 파라미터를 수식에 적용가능한 형태로 변환
  const float q = Dsp::Utils::GetResonanceQ(lpf_parameters.resonance.current);

  float x = 0, y = 0;
  for (std::size_t i = 0; i < size; i += FRAME_BUFFER_COUNT) {
    // 2. 수식에 필요한 변수를 업데이트
    const float cutoff = GetCutoffFrequency(
        lpf_parameters.rate, lpf_parameters.depth, lpf_parameters.cutoff);
    float omega_degree =
        360.0f * cutoff * AudioConfig::REVERSE_SAMPLING_FREQUENCY;
    float cos = Utils::CosineFromDegrees(omega_degree);
    float sin = Utils::SineFromDegrees(omega_degree);
    float alpha = sin / (2.0f * q);

    float reverse_a0 = 1 / (1.0f + alpha);
    b0 = ((1.0f - cos) * 0.5f) * reverse_a0;
    b1 = (1.0f - cos) * reverse_a0;
    b2 = ((1.0f - cos) * 0.5f) * reverse_a0;
    a1 = (-2.0f * cos) * reverse_a0;
    a2 = (1.0f - alpha) * reverse_a0;

    // 3. 버퍼에 필터 적용
    x = static_cast<float>(buffer[i + INMP441_ALIGN_OFFSET]);
    y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;

    buffer[i + INMP441_ALIGN_OFFSET] = static_cast<BitDepth_t>(y);
  }
}

float Lpf::GetCutoffFrequency(ParameterModel::RateValue& rate,
                              ParameterModel::UnsignedRangeValue& depth,
                              ParameterModel::UnsignedRangeValue& cutoff) {
  // 로그 스케일에서 연산하지 않고 0.0~1.0으로 정규화하여 연산
  const float c = (float)cutoff.current / (cutoff.max - cutoff.min);
  const float d = (float)depth.current / (depth.max - depth.min);

  const float a = d * d * d;                // 측정치를 근거로한 단순 근사
  const float h = std::fminf(c + a, 1.0f);  // 진동 범위의 최대값
  if (rate.IsMeasure()) {
    // TODO:
    // rate가 음표일 때 BPM을 얻어와서 계산하기
  } else {
    std::int16_t numeric_rate = rate.ToUnsignedRangeValue();
    if (numeric_rate > 0) {
      phase += Dsp::Utils::GetRateFrequency(numeric_rate) * AudioConfig::REVERSE_SAMPLING_FREQUENCY;
      phase -= std::floor(phase);
    } else {
      phase = 0;
    }
  }

  const float T = 1.0f - std::fabsf(2.0f * phase - 1.0f);  // 삼각파의 현재 위치
  const float x_cut =
      std::fmaxf(c, h - a + a * T);  // cutoff보다 낮은 값은 cutoff로 고정

  const float cutoff_hz =
      Dsp::Utils::HZ_MIN * powf(Dsp::Utils::HZ_MAX / Dsp::Utils::HZ_MIN, x_cut);
  return cutoff_hz;
}

}  // namespace DSP::FxProcessor
