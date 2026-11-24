#ifndef XL5050_RGBW_HPP
#define XL5050_RGBW_HPP

#include <cstdint>

#include "singleton.hpp"
#include "stm32h7xx.h"
#include "track_index.hpp"
#include "track_state_id.hpp"

namespace Xl5050Rgbw {

enum class Status {
  OK = 0,
  ERROR,
  BUSY,
};

enum class InitState {
  NOT_INITED = 0,
  ON_INIT,
  INITED,
};

using TimerCcr_t = std::uint16_t;

constexpr std::size_t TrackCount =
    1U;  // static_cast<std::size_t>(TrackIndex::COUNT);
constexpr std::size_t ColorCount = 3U;
constexpr std::size_t ColorBitCount = 8U;
constexpr std::size_t ResetLowCount = 80U;
constexpr std::size_t TimerCcrCount =
    TrackCount * ColorCount * ColorBitCount + ResetLowCount;

using ColorValue_t = std::uint8_t;

struct Color {
  ColorValue_t red;
  ColorValue_t green;
  ColorValue_t blue;
};

class Driver : public Singleton<Driver> {
  friend class Singleton<Driver>;

 private:
  Driver() {}
  ~Driver() {}

 public:
  Status Initialize(TIM_HandleTypeDef* htim);
  Status UpdateTrackLedStatus(TrackStateMachine::Id track_state_id,
                              TrackIndex track_index);

 private:
  TIM_HandleTypeDef* htim;
  InitState init_state;
  TrackStateMachine::Id track_state_ids[TrackCount];

  void ConvertTrackStateIdsToTimerCcrs();
  void ConvertColorValueToTimerCcr(ColorValue_t color_value,
                                   std::size_t base_index);
  Status Send();
};

}  // namespace Xl5050Rgbw

#endif
