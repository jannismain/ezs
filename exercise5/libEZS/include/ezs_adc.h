#ifndef EZS_ADC_H_INCLUDED
#define EZS_ADC_H_INCLUDED

/*!
 * @file ezs_adc.h
 * @brief A very simple AD converter, based on Fail*
 * @author Martin Hoffmann
 * \ingroup dac
 */

#include <stdint.h>

/*!
 * \brief Get a value from the ADC.
 */
uint8_t ezs_adc_read(void);


#endif // EZS_ADC_H_INCLUDED
