
/*!
 * @file ezs_adc.c
 * @brief A very simple AD converter, based on Fail*
 * @author Martin Hoffmann 
 * \ingroup adc 
 */
#include <stdint.h>
#include "ezs_dac.h"

static volatile uint8_t ezs_adc_in_register;
/*!
 * \brief Get a value from the ADC
 */
uint8_t ezs_adc_read(void){
  return ezs_adc_in_register;
}



