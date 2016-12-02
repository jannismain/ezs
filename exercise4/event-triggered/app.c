#include <cyg/hal/hal_arch.h>
#include <cyg/infra/diag.h>
#include <cyg/kernel/kapi.h>
#include <stdio.h>

#include "ezs_counter.h"
#include "ezs_trace.h"
#include "ezs_io_fel.h"
#include "ezs_sensor.h"
#include "ezs_dac.h"

#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+1024
// Thread 1
static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t threadhndl1;
static cyg_thread   threaddata;
static cyg_alarm    alarm1;
static cyg_handle_t alarmhnd1;

static cyg_handle_t counter;

void alarm_handler(cyg_handle_t alarm, cyg_addrword_t data)
{
	// 'data' is a pointer to a thread handle! (see cyg_alarm_create below)
	cyg_thread_resume(*((cyg_handle_t*) data));
}


//A little helper function.
cyg_tick_count_t ms_to_cyg_ticks(cyg_uint32 ms)
{
	/**
	 * Implement if needed!
	 **/
	return ms;
}

//A little helper function.
cyg_tick_count_t ms_to_ezs_ticks(cyg_uint32 ms)
{
	/**
	 * Implement if needed!
	 **/
	return ms;
}
volatile int x = CYG_FB_WIDTH(FRAMEBUF);

// A little test thread.
void thread(cyg_addrword_t arg)
{

	/**
	 * Example for the usage of the framebuffer.
	 * Comment out for simulating the SimpleScope!
	 * Reuse for displaying the pds!
	 **/
	int i;
	int xstart = 14*16 /* chars */ + 10 /* margin */;
	int sizex  = ( CYG_FB_WIDTH(FRAMEBUF) - xstart ) / 4;
	int sizey  = CYG_FB_HEIGHT(FRAMEBUF) /4;

	for( i = 0; i < 16; i++)
	{
		int col = sizex * (i % 4);
		int row = sizey * (i / 4);

		ezs_fb_fill_block(xstart + col, row, sizex, sizey, i);

	}
	int size = 16;
	for(i=0; i < 94; i++)
	{
		int row = size * (i % 14);
		int col = size * (i / 14);

		ezs_fb_print_char(i+'!', row+size, col+size,  CYG_FB_DEFAULT_PALETTE_BLUE);
	}

	ezs_fb_print_string("Hallo Welt!",60,150, CYG_FB_DEFAULT_PALETTE_RED);

	while(1)
	{
		/* Use this rather than printf, for EZS FrameBuffer Simulator 3000 */
		ezs_printf("Hallo!\n");
		cyg_thread_suspend(cyg_thread_self());
	}
}


void cyg_user_start(void)
{
	ezs_serial_init();
	ezs_sensors_init();
	ezs_dac_init();

	// Initialize framebuffer in graphic mode
	ezs_fb_init();

	// Initialize EZS counter
	ezs_counter_init();

	// Create test thread
	cyg_thread_create(11, &thread, 0, "Abtastung1", my_stack, STACKSIZE,
	                  &threadhndl1, &threaddata);

	cyg_clock_to_counter(cyg_real_time_clock(), &counter);

       /**
        * BEWARE! Neiter ms_to_ezs_ticks nor ms_to_cyg_ticks are working!
        * Fix them in order to complete this exercise
        */
	// Create alarm. Notice the pointer to the threadhndl1 as alarm function parameter!
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhndl1 , &alarmhnd1, &alarm1);
	cyg_alarm_initialize(alarmhnd1, cyg_current_time() + 1, ms_to_cyg_ticks(100));
	cyg_alarm_enable(alarmhnd1);

}

