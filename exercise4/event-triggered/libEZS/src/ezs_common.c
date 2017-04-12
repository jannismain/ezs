#include "ezs_common.h"

uint8_t ezs_low_byte(uint16_t value) {
    return (uint8_t) (value & 0xFF);
}

uint8_t ezs_high_byte(uint16_t value) {
    return (uint8_t) ((value & 0xFF00) >> 8);
}

