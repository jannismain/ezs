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
#define PRIORITY 10

static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t handle;
static cyg_thread threaddata;

void test_thread(cyg_addrword_t arg)
{
	/* Einmalige Aufgaben ... */

	int y_0 = 125;
	int f = 100;
	int offset = y_0;
	int t = 0; // time
	int d = 500; // delay 
	// int ms = 20;

	while(1)
	{
		/* Periodische Aufgaben ... */
        	float y = y_0 * sinf((2*M_PI*f*t)) + offset;
        	ezs_dac_write((uint8_t) y);
        	// ezs_printf("Time: %f, Value: %f\n", t, y);
        	ezs_delay_us(d*1000);
        	t ++;
	}

}


void cyg_user_start(void)
{
	ezs_gpio_init();
	ezs_dac_init();
	ezs_counter_init();

	/* Thread erzeugen ... */
    cyg_thread_create(PRIORITY, &test_thread, 0, "mythread", my_stack, STACKSIZE, &handle, &threaddata);

	/* Thread starten ... */
	cyg_thread_resume(handle);

    return;
}

