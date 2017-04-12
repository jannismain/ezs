#ifndef EZS_GPIO_H_INCLUDED
#define EZS_GPIO_H_INCLUDED

#include <stdbool.h>
/**
 * \file ezs_gpio.h
 * \ingroup hw
 * \brief Access to General Purpose Input/Output pins
 */

/**
 * \brief Call this function exactly once to initialize the GPIO system.
 * As long as this function has not been called, calls to other functions in
 * this module have undefined behaviour.
 */
void ezs_gpio_init(void);

/**
 * \brief Sets the GPIO pin to high (enabled == true) or low (enabled ==
 * false).
 */
void ezs_gpio_set(bool /* enabled */);

#endif // EZS_GPIO_H_INCLUDED
