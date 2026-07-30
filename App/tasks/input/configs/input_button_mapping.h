#ifndef INPUT_BUTTON_MAPPING_H
#define INPUT_BUTTON_MAPPING_H

#include <stdint.h>

#include "button_id.h"

typedef struct {
    uint8_t slave_address;
    ButtonId ids[16]; // first 8 is port A, second is port B
} InputButtonMapping;

extern const InputButtonMapping input_button_mappings[];
extern const uint8_t input_button_mapping_count;

#endif
