#ifndef SYSTEM_INITPARAMS_H
#define SYSTEM_INITPARAMS_H

#include "cmsis_os2.h"
#include "stm32h7xx.h"

typedef struct {
  I2C_HandleTypeDef* hi2c;
  osMutexId_t i2c1_mutex;
} Mcp23017InitParams;

typedef struct {
  SDRAM_HandleTypeDef* hsdram;
} SdramInitParams;

typedef struct {
  SPI_HandleTypeDef* hspi;
  I2C_HandleTypeDef* hi2c;
  osMutexId_t i2c1_mutex;
  GPIO_TypeDef* CS_Port;
  uint16_t CS_Pin;
  GPIO_TypeDef* RST_Port;
  uint16_t RST_Pin;
  GPIO_TypeDef* DC_Port;
  uint16_t DC_Pin;
} Gmg12864InitParams;

typedef struct {
  osEventFlagsId_t system_init_event;

  Mcp23017InitParams mcp23017_init_params;
  SdramInitParams sdram_init_params;
  Gmg12864InitParams gmg12864_init_params;
} SystemInitParams;

#endif
