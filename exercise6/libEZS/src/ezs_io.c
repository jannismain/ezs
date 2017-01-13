#include "ezs_io.h"
#include <stdio.h>
#include "ezs_counter.h"

void ezs_print_measurement(cyg_uint32 timestamp, cyg_uint32 value) {
    printf("time: %u value: %u\n", (unsigned int)timestamp, (unsigned int)value);
}

