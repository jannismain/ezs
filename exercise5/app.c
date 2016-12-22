#include <cyg/kernel/kapi.h>

#include <cyg/hal/hal_arch.h>

#include <stdio.h>

#include "ezs_counter.h"
#include "ezs_trace.h"

#define STACKSIZE (CYGNUM_HAL_STACK_SIZE_MINIMUM+1024)
// Thread 1
static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t threadhndl1;
static cyg_thread threaddata;

// Timer alarm
static cyg_alarm alarm1;
static cyg_handle_t alarmhnd1;

static cyg_handle_t counter;

//A little helper function
static cyg_tick_count_t ms_to_cyg_ticks(cyg_uint32 ms)
{
	/**
	 * Implement if needed!
	 **/
	return ms;
}

static cyg_tick_count_t ms_to_ezs_ticks(cyg_uint32 ms)
{
	/**
	 * Implement if needed!
	 **/
	return ms;
}

void cyg_user_start(void) {
	// Initialize EZS counter
	ezs_counter_init();

	/*
	 * ...
	 */
}
