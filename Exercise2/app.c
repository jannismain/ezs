#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include <stdio.h>
#include <iso646.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "ezs_counter.h"
#include "ezs_delay.h"
#include "ezs_gpio.h"
#include "ezs_stopwatch.h"
#include "ezs_serial.h"
#include "ezs_dac.h"
#include "ezs_io.h"

#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+10240

#define SERIAL_IRQ CYGNUM_HAL_INTERRUPT_UART2
static cyg_interrupt serial_intr;
static cyg_handle_t serial_isr_handle;

static volatile uint8_t input;

static cyg_uint32 serial_isr_handler(cyg_vector_t vector, cyg_addrword_t data)
{
	/* TODO: FIX ISR!!!! */
	if (ezs_serial_char_available())
	{
		input = ezs_serial_getc();
		cyg_interrupt_acknowledge(vector);
		ezs_serial_putc(input);
	}
	return CYG_ISR_HANDLED;
}

static void serial_dsr_handler(cyg_vector_t vector, cyg_ucount32 count,
                               cyg_addrword_t data)
{
	/* Implement */
}

static void serial_thread(cyg_addrword_t arg)
{
	/* Implement */
}

static void signal_generator_thread(cyg_addrword_t arg)
{
	/* Implement */
}

void cyg_user_start(void)
{
	// Setup counter
	ezs_counter_init();
	ezs_dac_init();
	ezs_serial_init();

	/* Create signal_generator_thread */
	/* Implement */

	/* Create serial_thread */
	/* Implement */

	// Create ISR + DSR
	cyg_interrupt_create(SERIAL_IRQ,
	                     1,
	                     0,
	                     serial_isr_handler,
	                     serial_dsr_handler,
	                     &serial_isr_handle,
	                     &serial_intr) ;
	cyg_interrupt_attach(serial_isr_handle);
	cyg_interrupt_unmask(SERIAL_IRQ);

    // test ezs_lose_time
    cyg_uint32 counter;
    cyg_uint32 diff = 0;
    int j;
    
    ezs_printf("measure overhead ezs_watch_*: "); 
    for (j =0; j < 100; j++) {
        ezs_watch_start(&counter);
        diff += ezs_watch_stop(&counter);
    }
    ezs_printf("Overhead = %u\n", diff/100);

    ezs_printf("Starting test ezs_lose_time\n");
    for (j = 250; j < 1000; j += 1) {
        ezs_watch_start(&counter);
        int i;
        for (i = 0; i < 100; i++) {
            ezs_lose_time(j, i);
        }
        diff = ezs_watch_stop(&counter);
        //	ezs_printf("expected: %d, actual: %d, diff: %d\n", j,diff/100,j-diff/100);
    }
    while(1);
}
