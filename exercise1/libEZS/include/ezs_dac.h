#ifndef EZS_DAC_H_INCLUDED
#define EZS_DAC_H_INCLUDED

/*!
 * @file ezs_dac.h
 * @brief A very simple DA converter. Implemented for tricore and for fail*
 * @authors Martin Hoffmann, Tobias Klaus
 * \ingroup dac
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Initializes the DAC. This function has to be called exactly once
 *        so ezs_dac_write() exhibits defined behaviour.
 */
void ezs_dac_init(void);

/*!
 * \brief Write a value to the DAC.
 */
void ezs_dac_write(uint8_t value);

#ifdef __cplusplus
}
#endif


#endif // EZS_DAC_H_INCLUDED
