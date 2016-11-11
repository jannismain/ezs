#ifndef EZS_SERIAL_H_INCLUDED
#define EZS_SERIAL_H_INCLUDED

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file ezs_serial.h
 * \ingroup hw
 * \brief Functions for using the SERIAL interface.
 */

/**
 * \brief initializes the SERIAL interface. Call this function exactly once
 * before using any other function provided by this module.
 */
void ezs_serial_init(void);



#ifdef EZS_DUMB_SERIAL
/**
 * \brief writes a single byte to the UART hardware register
 */
void ezs_serial_putc(uint8_t /* c */);

/**
 * \brief reads a single byte from the UART hardware register
 */
uint8_t ezs_serial_getc(void);

/**
 * \brief tells whether a byte is available on the UART
 */
uint8_t ezs_serial_char_available(void);
#else

/**
 * \brief reads a single byte from the UART input buffer, blocking if no byte is available.
 */
uint8_t ezs_serial_blocking_getc(void);
#endif

#ifdef __cplusplus
}
#endif

#endif // EZS_SERIAL_H_INCLUDED
