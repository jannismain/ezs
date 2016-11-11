/*!
 * @file ezs_dac.c
 * @brief A very simple DA converter, based on Fail*
 * @author Martin Hoffmann
 * \ingroup dac
 */
#include <stdint.h>
#include "ezs_dac.h"

static volatile uint8_t ezs_dac_out_register;

/*!
 * \brief Write a value to the DAC
 */
void ezs_dac_write(uint8_t value) {
    ezs_dac_out_register = value;
}

void ezs_dac_init() {
}

