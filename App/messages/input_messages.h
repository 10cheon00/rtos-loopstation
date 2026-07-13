#ifndef INPUT_MESSAGES_H
#define INPUT_MESSAGES_H

#include <stdint.h>

typedef struct {
  uint8_t mcp23017_id;
  uint8_t int_line;
  uint32_t timestamp_ms;
} Mcp23017IntEvent;

#endif
