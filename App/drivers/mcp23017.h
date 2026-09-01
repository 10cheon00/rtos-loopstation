#ifndef MCP23017_H
#define MCP23017_H

#include <stdint.h>

#include "stm32h7xx.h"

#include "mcp23017_gpio_map.h"

#define MCP23017_CONTROL_REGISTER_INTFA 0xE
#define MCP23017_CONTROL_REGISTER_INTFB 0xF
#define MCP23017_CONTROL_REGISTER_INTCAPA 0x10
#define MCP23017_CONTROL_REGISTER_INTCAPB 0x11

typedef enum {
    MCP23017_STATUS_OK = 0,
    MCP23017_STATUS_ERROR
} Mcp23017Status;

typedef struct {
    I2C_HandleTypeDef *hi2c;
} Mcp23017InitParams;

typedef uint16_t Mcp23017GpioInterruptPin;

typedef struct {
    Mcp23017Address address;
    Mcp23017GpioInterruptPin gpio_interrupt_pin;
} Mcp23017AddressInterruptPinMap;

Mcp23017Status Mcp23017_Init(Mcp23017InitParams *params);
Mcp23017Status Mcp23017_ReadRegister(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t reg,
                                     uint8_t *value);
Mcp23017Status Mcp23017_WriteRegister(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t reg,
                                      uint8_t value);
Mcp23017Status Mcp23017_UpdateOutputPinState(I2C_HandleTypeDef *hi2c, uint8_t address, uint8_t port,
                                             uint8_t pin_register_mask, uint8_t pin_state);
Mcp23017Status Mcp23017_GetMcp23017AddressFromInterruptPin(Mcp23017GpioInterruptPin gpio_interrupt_pin, uint8_t *address);


#endif
