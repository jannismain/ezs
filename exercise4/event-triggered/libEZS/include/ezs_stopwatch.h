/*!
 * @file ezs_stopwatch.h
 * A stopwatch for time measurement and
 * WCET generation.
 *
 * \ingroup hw
 *  @brief EZS time measurement and WCET generation
 *  @author Martin Hoffmann
 */

#ifndef __STOPWATCH_H_
#define __STOPWATCH_H_
#include <cyg/infra/cyg_type.h>

/**
 *  Start stop watch.
 *  @param state Pointer to a state variable
 */
void ezs_watch_start(cyg_uint32* state);

/**
 *  Stop watch and return time in timer's ticks.
 *  @param state Pointer to a state variable
 *  @see ezs_watch_start
 */
cyg_uint32 ezs_watch_stop(cyg_uint32* state);

/**
 *  Waits actively for WCET simulation.
 *  @param wceticks Desired amount of ticks to wait actively in in in ezs_counter ticks.
 *              Beware! Too small amounts of ticks may be undetectable.
 *              Thus this function may not support small granularities.
 *  @param percentage Maximal percentage of the wcet that might NOT be lost to better simulate real workload. Values above 100 are mapped to 100
 *
 * \see ezs_counter_get_resolution()
 */
void ezs_lose_time(cyg_uint32 wceticks, cyg_uint8 percentage);


#endif

