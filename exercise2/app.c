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

#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+1024
#define PRIORITY_SIN 15
#define PRIORITY_SERIAL_DSR 12
#define SERIAL_IRQ CYGNUM_HAL_INTERRUPT_UART2
static cyg_interrupt serial_intr;
static cyg_handle_t serial_isr_handle;

static volatile uint8_t input;

// sin thread variables
static cyg_uint8 stack_sin[STACKSIZE];
static cyg_handle_t handle_sin;
static cyg_thread threaddata_sin;

// serial_dsr thread variables
static cyg_uint8 stack_serial_dsr[STACKSIZE];
static cyg_handle_t handle_serial_dsr;
static cyg_thread threaddata_serial_dsr;

// timing variables
static cyg_uint32 interrupt_time;

static cyg_uint32 serial_isr_handler(cyg_vector_t vector, cyg_addrword_t data)
{
    ezs_watch_start(&interrupt_time);
	if (ezs_serial_char_available())
	{
		input = ezs_serial_getc();
		cyg_interrupt_acknowledge(vector);
		return CYG_ISR_CALL_DSR;
	}
	return CYG_ISR_HANDLED;
}

static void serial_thread(cyg_addrword_t arg)
{
	ezs_serial_putc(input);
	cyg_uint32 delay = ezs_watch_stop(&interrupt_time);
	printf(" (Zeitmessung Interrupt - Ausgabe: %u ms)\n", (delay*ezs_counter_resolution_ps()/1000000));
}

static void serial_dsr_handler(cyg_vector_t vector, cyg_ucount32 count,
                               cyg_addrword_t data)
{
    cyg_thread_resume(handle_serial_dsr);
}

static void signal_generator_thread(cyg_addrword_t arg)
{
	ezs_printf("Starting test_sin\n");

    // user input
	uint32_t y_0 = 50;
	uint32_t f1 = 2;
	uint32_t f2 = 13;
	uint32_t sampling_rate = 50; // in Hz, must be >2*max(f), where f=[f1, f2, .., fn]

	// calculated values
    float omega1 = f1 * M_PI * 2;
    float omega2 = f2 * M_PI * 2;
	uint32_t offset = 2 * y_0;
	uint32_t d = 1000/sampling_rate; // delay in ms to sample at sampling_rate
	// uint32_t ms = 20;
    uint32_t t = 0; // time in ms

	while(1)
	{
        	float y = y_0 * (sinf(omega1*(t/1000.)) + sinf(omega2*(t/1000.))) + offset;
        	ezs_dac_write((uint8_t) y);
//        	ezs_printf("Time: %u, Value: %f -> %u\n", t, y, (uint8_t)y);
        	ezs_delay_us(d * 1000); // in microseconds, d in milliseconds
        	t += d;
	}
}

// implementation of part 3
void test_lose_time(void)
{
	ezs_printf("Starting test ezs_lose_time\n");
    
    cyg_uint32 counter = 0;
	cyg_uint32 diff = 0;
	int j;

    for (j = 250; j < 1000; j += 1) {
        ezs_watch_start(&counter);
        int i;
        for (i = 0; i < 100; i++) {
            ezs_lose_time(j, i);
        }
        diff = ezs_watch_stop(&counter);
	//ezs_printf("expected: %d, actual: %d, diff: %d\n", j,diff1/100,j-diff1/100);
    }
}

// implementation of part 5
// see signal_generator_thread()


// implementation of part 11
void test_timing_overhead(int iterations)
{
    ezs_printf("measure overhead ezs_watch_*: ");
    
    cyg_uint32 counter = 0;
    cyg_uint32 diff = 0;
    int j;

    for (j = 0; j < iterations; j++) {
        ezs_watch_start(&counter);
        diff += ezs_watch_stop(&counter);
    }
    ezs_printf("Overhead = %u\n", diff/iterations);
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

    // part 3: test ezs_lose_time
    test_lose_time();

    // part 5: start signal thread
    cyg_thread_create(PRIORITY_SIN, &signal_generator_thread, 0, "thread_sin", stack_sin, STACKSIZE, &handle_sin, &threaddata_sin);
    cyg_thread_resume(handle_sin);
    
    // start dsr thread
    cyg_thread_create(PRIORITY_SERIAL_DSR, &serial_thread, 0, "thread_serial_dsr", stack_serial_dsr, STACKSIZE, &handle_serial_dsr, &threaddata_serial_dsr);

    // part 11:
    test_timing_overhead(100);
}
