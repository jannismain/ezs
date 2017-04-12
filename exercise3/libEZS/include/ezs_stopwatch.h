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
void ezs_watch_start(cyg_uint32 * state);

/**
 *  Stop watch and return time im the timer's ticks.
 *  @param state Pointer to a state variable
 *  @see ezs_watch_start
 */
cyg_uint32 ezs_watch_stop(cyg_uint32 * state);

/**
 *  Waits actively for WCET simulation. 
 *  @param wcet Desired time to wait actively in timer's ticks.
 *  @param percentage desired minimal execution time in percent
 */
void ezs_lose_time(cyg_uint32 wcet, cyg_uint8 percentage);


#endif

