/*!
 * @file ezs_delay.h
 * delays the execution of the program
 *
 * \ingroup hw
 *  @brief EZS delay
 *  @author Florian Jung
 */

#ifndef __DELAY_H_
#define __DELAY_H_
#include <cyg/infra/cyg_type.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 *  Delays execution for the desired amount of microseconds
 *  @param microseconds How many microseconds to wait
 *  @note ezs_counter_init() must have been called prior to use!
 */
void ezs_delay_us(cyg_uint32 microseconds);

#ifdef __cplusplus
}
#endif

#endif

