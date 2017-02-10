#ifndef EZS_COMMON_H_INCLUDED
#define EZS_COMMON_H_INCLUDED

/*!
 * @file ezs_common.h
 * @brief common functions for EZS
 * @author Florian Franzmann
 * \ingroup common
 */

#include <stdint.h>



/*!
 * \brief returns the low byte of value
 */
uint8_t ezs_low_byte(uint16_t value);

/*!
 * \brief returns the high byte of value
 */
uint8_t ezs_high_byte(uint16_t value);


#endif // EZS_COMMON_H_INCLUDED
