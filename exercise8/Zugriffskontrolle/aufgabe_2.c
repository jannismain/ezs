#include <cyg/kernel/kapi.h>
#include <stdio.h>

#include "ezs_stopwatch.h"
#include "ezs_io.h"

#include "common.h"

// in milliseconds
#define T1_TASK_PERIOD 20
#define T2_TASK_PERIOD 50
#define T3_TASK_PERIOD 100
#define T4_TASK_PERIOD 200


#define T1_TASK_PHASE 7
#define T2_TASK_PHASE 5
#define T3_TASK_PHASE 3
#define T4_TASK_PHASE 1

#define T1_TASK_PRIORITY 4
#define T2_TASK_PRIORITY 5
#define T3_TASK_PRIORITY 6
#define T4_TASK_PRIORITY 7


#define PERCENTAGE  0

cyg_mutex_t r2;
cyg_mutex_t r3;
cyg_mutex_t r4;

cyg_handle_t  t1_task_handle;
cyg_handle_t  t2_task_handle;
cyg_handle_t  t3_task_handle;
cyg_handle_t  t4_task_handle;

static cyg_uint8     t1_task_stack[STACKSIZE];
static cyg_thread    t1_task_thread;

static cyg_uint8     t2_task_stack[STACKSIZE];
static cyg_thread    t2_task_thread;

static cyg_uint8     t3_task_stack[STACKSIZE];
static cyg_thread    t3_task_thread;

static cyg_uint8     t4_task_stack[STACKSIZE];
static cyg_thread    t4_task_thread;

static void t1_task_entry(cyg_addrword_t data);
static void t2_task_entry(cyg_addrword_t data);
static void t3_task_entry(cyg_addrword_t data);
static void t4_task_entry(cyg_addrword_t data);

static void t1_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(t1_task_handle);
}

static void t2_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(t2_task_handle);
}

static void t3_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(t3_task_handle);
}

static void t4_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(t4_task_handle);
}

static cyg_handle_t s_t1_task_alarm_handle;
static cyg_alarm s_t1_task_alarm;
static cyg_handle_t s_t2_task_alarm_handle;
static cyg_alarm s_t2_task_alarm;
static cyg_handle_t s_t3_task_alarm_handle;
static cyg_alarm s_t3_task_alarm;
static cyg_handle_t s_t4_task_alarm_handle;
static cyg_alarm s_t4_task_alarm;



void init_tasks(void)
{
    
    cyg_mutex_init(&r2);
    cyg_mutex_init(&r3);
    cyg_mutex_init(&r4);

    cyg_mutex_set_protocol(&r2, CYG_MUTEX_CEILING);
    cyg_mutex_set_protocol(&r3, CYG_MUTEX_CEILING);
    cyg_mutex_set_protocol(&r4, CYG_MUTEX_CEILING);

	cyg_thread_create(T1_TASK_PRIORITY, &t1_task_entry, 0, "t1",
			t1_task_stack, STACKSIZE,
			&t1_task_handle, &t1_task_thread);
	cyg_thread_create(T2_TASK_PRIORITY, &t2_task_entry, 0, "t2",
			t2_task_stack, STACKSIZE,
			&t2_task_handle, &t2_task_thread);
	cyg_thread_create(T3_TASK_PRIORITY, &t3_task_entry, 0, "t3",
			t3_task_stack, STACKSIZE,
			&t3_task_handle, &t3_task_thread);
	cyg_thread_create(T4_TASK_PRIORITY, &t4_task_entry, 0, "t4",
			t4_task_stack, STACKSIZE,
			&t4_task_handle, &t4_task_thread);

	cyg_alarm_create(s_real_time_counter, t1_task_alarmfn, data_dummy, &s_t1_task_alarm_handle, &s_t1_task_alarm);
	cyg_alarm_initialize(s_t1_task_alarm_handle, T1_TASK_PHASE, T1_TASK_PERIOD);
	cyg_alarm_create(s_real_time_counter, t2_task_alarmfn, data_dummy, &s_t2_task_alarm_handle, &s_t2_task_alarm);
	cyg_alarm_initialize(s_t2_task_alarm_handle, T2_TASK_PHASE, T2_TASK_PERIOD);
	cyg_alarm_create(s_real_time_counter, t3_task_alarmfn, data_dummy, &s_t3_task_alarm_handle, &s_t3_task_alarm);
	cyg_alarm_initialize(s_t3_task_alarm_handle, T3_TASK_PHASE, T3_TASK_PERIOD);
	cyg_alarm_create(s_real_time_counter, t4_task_alarmfn, data_dummy, &s_t4_task_alarm_handle, &s_t4_task_alarm);
	cyg_alarm_initialize(s_t4_task_alarm_handle, T4_TASK_PHASE, T4_TASK_PERIOD);
}

static void t1_task_entry(cyg_addrword_t data)
{
	while (1) {
        lose_time_us(1000, PERCENTAGE);
//        cyg_mutex_lock(&r2);
        cyg_scheduler_lock();
        lose_time_us(1000, PERCENTAGE);
        cyg_scheduler_unlock();
//        cyg_mutex_unlock(&r2);
		cyg_thread_suspend(cyg_thread_self());
	}
}

static void t2_task_entry(cyg_addrword_t data)
{
	while (1) {
        lose_time_us(1000, PERCENTAGE);
//        cyg_mutex_lock(&r2);
        cyg_scheduler_lock();
        lose_time_us(2000, PERCENTAGE);
//        cyg_mutex_lock(&r3);
        lose_time_us(2000, PERCENTAGE);
//        cyg_mutex_unlock(&r3);
        lose_time_us(1000, PERCENTAGE);
//        cyg_mutex_unlock(&r2);
        cyg_scheduler_unlock();
		cyg_thread_suspend(cyg_thread_self());
	}
}

static void t3_task_entry(cyg_addrword_t data)
{
	while (1) {
	    lose_time_us(1000, PERCENTAGE);
        cyg_scheduler_lock();
 //       cyg_mutex_lock(&r3);
        lose_time_us(3000, PERCENTAGE);
//        cyg_mutex_lock(&r4);
        lose_time_us(2000, PERCENTAGE);
//        cyg_mutex_unlock(&r3);
//        cyg_mutex_unlock(&r4);
        cyg_scheduler_unlock();
		cyg_thread_suspend(cyg_thread_self());
	}
}

static void t4_task_entry(cyg_addrword_t data)
{
	while (1) {
        lose_time_us(1000, PERCENTAGE);
        cyg_scheduler_lock();
//        cyg_mutex_lock(&r4);
        lose_time_us(9000, PERCENTAGE);
//        cyg_mutex_unlock(&r4);
        cyg_scheduler_unlock();
		cyg_thread_suspend(cyg_thread_self());
	}
}

