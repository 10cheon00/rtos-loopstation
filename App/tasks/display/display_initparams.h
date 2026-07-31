#ifndef DISPLAY_INITPARAMS_H
#define DISPLAY_INITPARAMS_H

#include "stm32h7xx.h"
#include "cmsis_os2.h"

typedef struct {
    osMessageQueueId_t display_snapshot_mailbox;
    SPI_HandleTypeDef *hspi;
    I2C_HandleTypeDef *hi2c;
    GPIO_TypeDef* CS_Port;
    uint16_t CS_Pin;
    GPIO_TypeDef* RST_Port;
    uint16_t RST_Pin;
    GPIO_TypeDef* DC_Port;
    uint16_t DC_Pin;
} DisplayInitParams;

#endif
