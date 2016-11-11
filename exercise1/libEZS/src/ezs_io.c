#include "ezs_io.h"
#include <stdio.h>
#include "ezs_counter.h"

void ezs_print_measurement(cyg_uint32 timestamp, cyg_uint32 value) {
    printf("time: %u ticks:\t delay: %u ticks\t(%.2fus)\n", (unsigned int)timestamp, (unsigned int)value, ((float)value)*ezs_counter_resolution_ps()/1000000.f );
}

