#ifndef EZS_COUNTER_H_INCLUDED
#define EZS_COUNTER_H_INCLUDED

#include <cyg/kernel/kapi.h>
/*!
 * @file ezs_counter.h
 * @brief A simple counter interface. Implemented for tricore and for fail*
 * @authors Martin Hoffmann, Tobias Klaus
 * \ingroup counter
 */

#include <cyg/infra/cyg_type.h>

#ifdef __cplusplus
extern "C" {
#endif
/*!
 * \brief This function has to be called exactly once so ezs_counter_get()
 *        yields meaningfull results.
 */
void ezs_counter_init(void);

/**
 * \brief Read the current counter value from the hardware counter.
 * @return counter value for time resolution
 *
 * \see ezs_counter_get_resolution
 */
cyg_uint32 ezs_counter_get(void);

/**
 * \brief Get the resolution of the counter referenced by ezs_counter_get()
 * @return resolution represented by a struct comprised of .dividend and .divsior
 *         each as cyg_uint32 types. dividend/divisor gives the nanoseconds per ticks.
 *
 * \see <http://ecos.sourceware.org/docs-latest/ref/kernel-clocks.html>
 **/

cyg_resolution_t ezs_counter_get_resolution(void);

/**
 * \brief Returns the current time resolution of the hardware counter.
 * @return tick resolution in picoseconds
 */
cyg_uint32 ezs_counter_resolution_ps(void);

#ifdef __cplusplus
}
#endif

#endif // EZS_COUNTER_H_INCLUDED
