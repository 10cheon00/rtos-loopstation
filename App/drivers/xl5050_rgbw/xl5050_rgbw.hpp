#ifndef XL5050_RGBW_HPP
#define XL5050_RGBW_HPP

#include <cstdint>
#include "singleton.hpp"
#include "stm32h7xx.h"
#include "track_index.hpp"

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

struct ColorBits {
    std::uint8_t green;
    std::uint8_t rgb;
    std::uint8_t blue;
};

class Driver : public Singleton<Driver> {
  friend class Singleton<Driver>;

 private:
  Driver() {}
  ~Driver() {}

 public:
  Status Initialize(TIM_HandleTypeDef* htim);
  Status UpdateTrackColorBits(ColorBits& color_bits, TrackIndex track_index);
  Status SendTrackColorBits();
 
 private:
  InitState init_state;
  ColorBits track_color_bits[static_cast<std::size_t>(TrackIndex::COUNT)];

  TIM_HandleTypeDef* htim;
};

}

#endif
