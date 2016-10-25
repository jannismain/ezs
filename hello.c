#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <iso646.h>

#include "ezs_counter.h"
#include "ezs_dac.h"
#include "ezs_delay.h"
#include "ezs_dac.h"
#include "ezs_gpio.h"
#include "ezs_io.h"

/* Thread-Stack */
#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+1024
static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t handle;
static cyg_thread threaddata;

void test_thread(cyg_addrword_t arg)
{
	ezs_printf("Hallo Welt!\n");
	/* Einmalige Aufgaben ... */

	while(1)
	{
		/* Periodische Aufgaben ... */

	}

}


void cyg_user_start(void)
{
	ezs_gpio_init();
	ezs_dac_init();
	ezs_counter_init();

	/* Thread erzeugen ... */


	/* Thread starten ... */
}

