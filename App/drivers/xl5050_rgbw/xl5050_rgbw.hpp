#ifndef XL5050_RGBW_HPP
#define XL5050_RGBW_HPP

#include <cstdint>

#include "singleton.hpp"
#include "stm32h7xx.h"
#include "track_config.h"
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

constexpr std::size_t ColorCount = 3U;
constexpr std::size_t ColorBitCount = 8U;
constexpr std::size_t ResetLowCount = 80U;
constexpr std::size_t TimerCcrCount =
    TRACK_COUNT * ColorCount * ColorBitCount + ResetLowCount;

using ColorValue_t = std::uint8_t;

struct Color {
  ColorValue_t red;
  ColorValue_t green;
  ColorValue_t blue;
};

struct TrackStateLedContext {
  TrackStateMachine::Id id;
  uint16_t* red_led_timer_ccr_address;
  uint16_t* green_led_timer_ccr_address;
  uint16_t* blue_led_timer_ccr_address;
};

class Driver : public Singleton<Driver> {
  friend class Singleton<Driver>;

 private:
  Driver() {}
  ~Driver() {}

 public:
  Status Initialize(TIM_HandleTypeDef* htim);
  Status UpdateTrackLedStatus(TrackStateMachine::Id track_state_id,
                              std::size_t track_index);

 private:
  TIM_HandleTypeDef* htim;
  InitState init_state;
  TrackStateMachine::Id track_state_ids[TRACK_COUNT];
  TrackStateLedContext track_state_led_contexts[TRACK_COUNT];

  void ConvertTrackStateIdsToTimerCcrs();
  void UpdateColorValueToTimerCcr(uint16_t* timer_ccr_address,
                                  ColorValue_t color_value);

  Status Send();
};

}  // namespace Xl5050Rgbw

#endif
