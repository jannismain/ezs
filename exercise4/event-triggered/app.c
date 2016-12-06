#include <cyg/hal/hal_arch.h>
#include <cyg/infra/diag.h>
#include <cyg/kernel/kapi.h>
#include <stdio.h>

#include "ezs_counter.h"
#include "ezs_trace.h"
#include "ezs_io_fel.h"
#include "ezs_sensor.h"
#include "ezs_dac.h"
#include "ezs_stopwatch.h"

#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+1024
// Thread 1
/*
static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t threadhndl1;
static cyg_thread   threaddata;
static cyg_alarm    alarm1;
static cyg_handle_t alarmhnd1;
*/

/* handles for threads */
static cyg_handle_t threadhandle_1;
static cyg_handle_t threadhandle_2;
static cyg_handle_t threadhandle_3;
static cyg_handle_t threadhandle_4;

/* stacks for threads */
static cyg_uint8 stack_1[STACKSIZE];
static cyg_uint8 stack_2[STACKSIZE];
static cyg_uint8 stack_3[STACKSIZE];
static cyg_uint8 stack_4[STACKSIZE];

/* thread data */
static cyg_thread   threaddata_1;
static cyg_thread   threaddata_2;
static cyg_thread   threaddata_3;
static cyg_thread   threaddata_4;

/* alarms */
static cyg_alarm	alarm_1;
static cyg_alarm	alarm_2;
static cyg_alarm	alarm_3;
static cyg_alarm	alarm_4;

/* alarm handles */
static cyg_handle_t alarmhandle_1;
static cyg_handle_t alarmhandle_2;
static cyg_handle_t alarmhandle_3;
static cyg_handle_t alarmhandle_4;

static cyg_handle_t counter;

void alarm_handler(cyg_handle_t alarm, cyg_addrword_t data)
{
	// 'data' is a pointer to a thread handle! (see cyg_alarm_create below)
	cyg_thread_resume(*((cyg_handle_t*) data));
}


//A little helper function.
cyg_tick_count_t ms_to_cyg_ticks(cyg_uint32 ms)
{
	cyg_handle_t clock = cyg_real_time_clock();
	cyg_resolution_t res = cyg_clock_get_resolution(clock);
	cyg_uint64 ticks = 1000000 * ms * (uint64_t)res.divisor / res.dividend;
	return (cyg_tick_count_t)ticks;
}

//A little helper function.
cyg_tick_count_t ms_to_ezs_ticks(cyg_uint32 ms)
{
	cyg_resolution_t res = ezs_counter_get_resolution();
	cyg_uint64 ticks = 1000000 * ms * (uint64_t)res.divisor / res.dividend;
	return (cyg_tick_count_t)ticks;
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

void thread_1(cyg_addrword_t arg)
{
	cyg_uint32 ms = 2;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	while(1) {
		cyg_thread_suspend(threadhandle_1);
		ezs_lose_time(ticks, 100);
	}
}

void thread_2(cyg_addrword_t arg)
{
	cyg_uint32 ms = 2;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	while(1) {
		cyg_thread_suspend(threadhandle_2);
		ezs_lose_time(ticks, 100);
	}
}

void thread_3(cyg_addrword_t arg)
{
	cyg_uint32 ms = 3;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	while(1) {
		cyg_thread_suspend(threadhandle_3);
		ezs_lose_time(ticks, 100);
	}
}

void thread_4(cyg_addrword_t arg)
{
	cyg_uint32 ms = 6;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	while(1) {
		cyg_thread_suspend(threadhandle_4);
		ezs_lose_time(ticks, 100);
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
//	cyg_thread_create(11, &thread, 0, "Abtastung1", my_stack, STACKSIZE,
//	                  &threadhndl1, &threaddata);

	cyg_clock_to_counter(cyg_real_time_clock(), &counter);

	// Create alarm. Notice the pointer to the threadhndl1 as alarm function parameter!
/*	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhndl1 , &alarmhnd1, &alarm1);
	cyg_alarm_initialize(alarmhnd1, cyg_current_time() + 1, ms_to_cyg_ticks(100));
	cyg_alarm_enable(alarmhnd1);
*/

	/* create threads */
	cyg_thread_create(1, thread_1, 0, "Thread_1", stack_1, STACKSIZE, &threadhandle_1, &threaddata_1);
	cyg_thread_create(3, thread_2, 0, "Thread_2", stack_2, STACKSIZE, &threadhandle_2, &threaddata_2);
	cyg_thread_create(4, thread_3, 0, "Thread_3", stack_3, STACKSIZE, &threadhandle_3, &threaddata_3);
	cyg_thread_create(5, thread_4, 0, "Thread_4", stack_4, STACKSIZE, &threadhandle_4, &threaddata_4);

	/* create alarms */
	// reicht ein counter fuer alle alarme???	
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhandle_1, &alarmhandle_1, &alarm_1);
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhandle_2, &alarmhandle_2, &alarm_2);
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhandle_3, &alarmhandle_3, &alarm_3);
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhandle_4, &alarmhandle_4, &alarm_4);

	/* initialize alarms */
	cyg_alarm_initialize(alarmhandle_1, cyg_current_time() + 1, ms_to_cyg_ticks(10));
	cyg_alarm_initialize(alarmhandle_2, cyg_current_time() + 1, ms_to_cyg_ticks(20));
	cyg_alarm_initialize(alarmhandle_3, cyg_current_time() + 1, ms_to_cyg_ticks(20));
	cyg_alarm_initialize(alarmhandle_4, cyg_current_time() + 1, ms_to_cyg_ticks(100));

	/* enable alarms */
	cyg_alarm_enable(alarmhandle_1);
	cyg_alarm_enable(alarmhandle_2);
	cyg_alarm_enable(alarmhandle_3);
	cyg_alarm_enable(alarmhandle_4);
}

