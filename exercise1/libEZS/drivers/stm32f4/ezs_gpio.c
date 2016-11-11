#include <cyg/hal/hal_arch.h>
#include "ezs_gpio.h"

#define EZS_GPIO CYGHWR_HAL_STM32F4DISCOVERY_LED1

void ezs_gpio_init(void)
{
	CYGHWR_HAL_STM32_GPIO_SET(EZS_GPIO);
}

void ezs_gpio_set(bool enable)
{
	CYGHWR_HAL_STM32_GPIO_OUT(EZS_GPIO, enable!=0);
}
