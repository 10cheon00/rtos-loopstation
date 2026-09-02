#ifndef GMG12864_LCD_H
#define GMG12864_LCD_H

#include <stdint.h>

#include "stm32h7xx.h"
#include "u8g2.h"

namespace Gmg12864 {

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

enum class LcdStatus { OK = 0, ERROR };

struct InitParams {
  SPI_HandleTypeDef* hspi;
  uint16_t CS_Pin;
  uint16_t RST_Pin;
  uint16_t DC_Pin;
  GPIO_TypeDef* CS_Port;
  GPIO_TypeDef* RST_Port;
  GPIO_TypeDef* DC_Port;
};

LcdStatus Init(u8g2_t* u8g2, InitParams* init_params);

}  // namespace Gmg12864

#endif
