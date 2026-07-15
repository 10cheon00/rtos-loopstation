#ifndef MCP23017_H
#define MCP23017_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx.h"

typedef enum {
    MCP23017_STATUS_OK = 0,
    MCP23017_STATUS_ERROR
} Mcp23017Status;

typedef struct {
    const uint8_t address;
    const uint8_t pin_status;
} Mcp23017Infomation;

typedef struct {
    I2C_HandleTypeDef *hi2c;
    const Mcp23017Infomation* infomations;
    const uint8_t infomation_len;
} Mcp23017InitParams;


Mcp23017Status Mcp23017_Init(Mcp23017InitParams *params);
Mcp23017Status Mcp23017_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t reg,
                                     uint8_t *value);
Mcp23017Status Mcp23017_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t reg,
                                      uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
