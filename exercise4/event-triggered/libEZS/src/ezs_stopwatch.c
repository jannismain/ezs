#include "ezs_stopwatch.h"
#include "ezs_counter.h"
#include <stdlib.h>

void ezs_watch_start(cyg_uint32* state) {
    *state = ezs_counter_get();
}

cyg_uint32 ezs_watch_stop(cyg_uint32* state) {
    return  ezs_counter_get() - (*state);
}


void ezs_lose_time(cyg_uint32 wcet, cyg_uint8 percentage) {
    cyg_uint32 previous_value = ezs_counter_get();
    cyg_uint8 percent = 0;
    const cyg_uint32 resolution = 30U;
    cyg_uint32 temp = 0;
    cyg_uint32 diff = 0;
    if (percentage > 0 && percentage <= 100 ) {
        percent = rand() % percentage;
    }
    cyg_uint32 counter = ( wcet - (wcet * percent) / 100);

    while (counter > resolution) {
        temp = ezs_counter_get();
        diff = temp - previous_value;

        // if we were not interrupted
        if (diff < resolution) {
            counter = counter - diff;
        }
        else {
	  counter = counter - resolution;
	}
        previous_value = temp;
    }
}

