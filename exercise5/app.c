#include <cyg/kernel/kapi.h>

#include <cyg/hal/hal_arch.h>

#include <stdio.h>
#include <cyg/infra/diag.h>
#include "ezs_counter.h"
#include "ezs_trace.h"
#include "ezs_io_fel.h"
#include "ezs_sensor.h"
#include "ezs_dac.h"
#include "ezs_stopwatch.h"

#define STACKSIZE (CYGNUM_HAL_STACK_SIZE_MINIMUM+1024)
// Thread 1
static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t threadhndl1;
static cyg_thread threaddata;

// Timer alarm
static cyg_alarm alarm1;
static cyg_handle_t alarmhnd1;

static cyg_handle_t counter;

volatile cyg_uint32 acc_t1;
volatile cyg_uint32 acc_t2_a;
volatile cyg_uint32 acc_t2_b;
volatile cyg_uint32 acc_t3;
volatile cyg_uint32 acc_t4;

volatile cyg_uint32 timer = 0;

static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t threadhndl1;
static cyg_thread   threaddata;

//A little helper function
cyg_tick_count_t ms_to_cyg_ticks(cyg_uint32 ms)
{
	cyg_handle_t clock = cyg_real_time_clock();
	cyg_resolution_t res = cyg_clock_get_resolution(clock);
	cyg_uint64 ticks = 1000000 * ms * (uint64_t)res.divisor / res.dividend;
	return (cyg_tick_count_t)ticks;
}

//a little helper function.
cyg_tick_count_t ms_to_ezs_ticks(cyg_uint32 ms)
{
	cyg_resolution_t res = ezs_counter_get_resolution();
	cyg_uint64 ticks = 1000000 * ms * (uint64_t)res.divisor / res.dividend;
	return (cyg_tick_count_t)ticks;
}

void task_1()
{
    ezs_dac_write(10);
	cyg_uint32 ms = 5;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	ezs_lose_time(ticks, 100);
    ezs_dac_write(0);
}

void task_2_a()
{
    ezs_dac_write(20);
	cyg_uint32 ms = 5;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	ezs_lose_time(ticks, 100);
    ezs_dac_write(0);
}

void task_2_b()
{
    ezs_dac_write(30);
	cyg_uint32 ms = 5;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	ezs_lose_time(ticks, 100);
    ezs_dac_write(0);
}

void task_3()
{
    ezs_dac_write(40);
	cyg_uint32 ms = 3;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	ezs_lose_time(ticks, 100);
    ezs_dac_write(0);
}
void task_4()
{
    ezs_dac_write(50);
	cyg_uint32 ms = 2;
	cyg_uint32 ticks = ms_to_ezs_ticks(ms);
	ezs_lose_time(ticks, 100);
    ezs_dac_write(0);
}

void thread(cyg_addrword_t data) {
    while(1) {
        if (acc_t4) {
            acc_t4 = false;
            task_4();
        }
        else if (acc_t1) {
            acc_t1 = false;
            task_1();
        }
        else if (acc_t3) {
            acc_t3 = false;
            task_3();
        }
        else if (acc_t2_a) {
            acc_t2_a = false;
            task_2_a();
        }
        else if (acc_t2_b) {
            acc_t2_b = false;
            task_2_b();
        }
        cyg_thread_suspend(threadhndl1);        
    }
}

void alarm_handler(cyg_handle_t alarm, cyg_addrword_t data) 
{
    timer++;
    if (timer % 10 == 0) acc_t4 = true;
    if (timer % 20 == 0) acc_t1 = true;
    if (timer % 20 == 0) acc_t3 = true;
    if (timer % 20 == 0) acc_t2_a = true;
    if (timer % 20 == 0) acc_t2_b = true;
    if (timer >= 40) timer = 0;
    cyg_thread_resume(threadhndl1);
}

void cyg_user_start(void) {
	// Initialize EZS counter
	ezs_counter_init();

	cyg_thread_create(1, &thread, 0, "lopp", my_stack, STACKSIZE,
	                  &threadhndl1, &threaddata);
	cyg_alarm_create(counter, alarm_handler, NULL, &alarmhnd1, &alarm1);
	cyg_alarm_initialize(alarmhnd1, cyg_current_time() + 1, ms_to_cyg_ticks(1));
}
