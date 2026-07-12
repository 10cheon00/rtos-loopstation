#ifndef MCP23017_H
#define MCP23017_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MCP23017_STATUS_OK = 0,
    MCP23017_STATUS_ERROR
} Mcp23017Status;

Mcp23017Status Mcp23017_Init(uint8_t address);
Mcp23017Status Mcp23017_ReadRegister(uint8_t address, uint8_t reg, uint8_t *value);
Mcp23017Status Mcp23017_WriteRegister(uint8_t address, uint8_t reg, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
