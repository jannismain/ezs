#include "ezs_stopwatch.h"
#include "ezs_counter.h"
#include <stdlib.h>
#include "ezs_io.h"
#include "ezs_dac.h"

#define MAX_TICKS_PER_CYCLE 30

void ezs_watch_start(cyg_uint32* state)
{
    *state = ezs_counter_get();
}

cyg_uint32 ezs_watch_stop(cyg_uint32* state)
{
    return ezs_counter_get() - *state;
}


void ezs_lose_time(cyg_uint32 wceticks, cyg_uint8 percentage)
{
    /**
     * Extend this function to handle interrupts and preemption!
     *
     * Currently it takes the "real-time" into account, not the
     * execution time! This has to be changed!
     *
     * First ignore parameter percentage!
     * Add its funtiionality when everything else works.
     */
    cyg_uint32 previous_value = ezs_counter_get();
    /**
     * Volunatry additional task:
     * Why do we use cyg_int64 here instead of cyg_uint32?
     * Is it possible to implement the final ezs_lose_time with
     * an internal cyg_uint32 counter?
     **/
    cyg_int64 counter = wceticks-MAX_TICKS_PER_CYCLE-21; // ermittelter overhead von lose_time()
    cyg_uint32 current_value = 0;
    cyg_uint32 diff = 0;

    while (counter > 0)
    {
        current_value = ezs_counter_get();
        //ezs_dac_write((uint8_t)counter);
        diff = current_value - previous_value;
        if (diff > MAX_TICKS_PER_CYCLE) {
            counter -= MAX_TICKS_PER_CYCLE;
        } else {
            counter = counter - diff;
        }
        previous_value = current_value;
    }
    //ezs_printf("%d\n", counter);
}

