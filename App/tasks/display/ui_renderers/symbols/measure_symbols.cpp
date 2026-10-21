#include "measure_symbols.hpp"

#include "gmg12864_lcd.hpp"

namespace UiRenderer::Symbol {

// 각 행은 1바이트이며, 하위 5비트를 사용합니다.
// XBMP 형식: 최하위 비트(LSB)가 왼쪽 픽셀입니다.

static constexpr uint8_t noteWhole[6] = {
    0x00,  // .....
    0x00,  // .....
    0x0E,  // .###.
    0x11,  // #...#
    0x11,  // #...#
    0x0E,  // .###.
};

static constexpr uint8_t noteHalf[6] = {
    0x08,  // ...#.
    0x08,  // ...#.
    0x08,  // ...#.
    0x0E,  // .###.
    0x09,  // #..#.
    0x06   // .##..
};

static constexpr uint8_t noteQuarter[6] = {
    0x04,  // ..#..
    0x04,  // ..#..
    0x04,  // ..#..
    0x04,  // ..#..
    0x07,  // ###..
    0x03   // ##...
};

static constexpr uint8_t noteEighth[6] = {
    0x0C,  // ..##.
    0x14,  // ..#.#
    0x04,  // ..#..
    0x04,  // ..#..
    0x07,  // ###..
    0x03   // ##...
};

static constexpr uint8_t noteSixteenth[6] = {
    0x0C,  // ..##.
    0x14,  // ..#.#
    0x0C,  // ..##.
    0x14,  // ..#.#
    0x07,  // ###..
    0x03   // ##...
};

void RenderMeasureSymbol(Fx::Measure measure, std::uint8_t cx,
                         std::uint8_t by) {
  Gmg12864::Driver& driver = Gmg12864::Driver::GetInstance();
  std::uint8_t const* bitmap = noteWhole;
  switch (measure) {
    case Fx::Measure::SIXTEENTH:
      bitmap = noteSixteenth;
      break;
    case Fx::Measure::EIGHTH:
      bitmap = noteEighth;
      break;
    case Fx::Measure::QUARTER:
      bitmap = noteQuarter;
      break;
    case Fx::Measure::HALF:
      bitmap = noteHalf;
      break;
    case Fx::Measure::WHOLE:
      bitmap = noteWhole;
      break;
    default:
      break;
  }

  driver.drawXBMP(cx - 2, by - 5, 5, 6, bitmap);
}

}  // namespace UiRenderer::Symbol