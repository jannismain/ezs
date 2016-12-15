#include <cyg/hal/hal_arch.h>
#include <cyg/infra/diag.h>
#include <cyg/kernel/kapi.h>
#include <stdio.h>

#include "ezs_adc.h"
#include "ezs_counter.h"
#include "ezs_trace.h"
#include "ezs_io_fel.h"
#include "ezs_sensor.h"
#include "ezs_dac.h"
#include "ezs_stopwatch.h"
#include "ezs_fft.h"
#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+1024

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

/* data */
#define LDS_NUM	4
#define LDS_INPUT_SIZE 64
#define LDS_OUTPUT_SIZE (LDS_INPUT_SIZE / 2)	// -1 ?????
#define ADC_DATA_SIZE (2 * LDS_INPUT_SIZE)

static volatile cyg_uint32 adc_wr = 0;
static volatile cyg_uint32 adc_rd = 0;
static volatile bool adc_data_full = false;
static float adc_data[ADC_DATA_SIZE];

static cyg_uint32 lds_in_wr = 0;
static float lds_input[LDS_INPUT_SIZE];

static volatile cyg_uint32 lds_wr = 0;
static volatile cyg_uint32 lds_rd = 0;
static volatile bool lds_full;
static float lds_output[LDS_INPUT_SIZE * LDS_NUM];



// For Debugging
static uint32_t test;
static uint32_t test_input() {
	test++;
    return test;
}

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
	while(1) {
		cyg_thread_suspend(threadhandle_2);

		//TODO: Synchronisation benoetigt??? Wenn ja, welches Verhalten erwartet
		//		bei schreibzugriff auf vollen/lesezugriff auf leeren buffer?
		if (adc_data_full)	/* dont fetch new data if buffer is full -> is this needed??? */
			continue;

		uint8_t x = ezs_adc_get();
        float value = ((float)x / 255) - 0.5;
//		float value = (float)test_input();
		adc_data[adc_wr] = value;
		adc_wr = (adc_wr + 1) % ADC_DATA_SIZE;

		/* check if buffer is full -> should not happen??? */
		if (adc_wr == adc_rd)
			adc_data_full = true;
	}
}

static float exp = 1;
void thread_3(cyg_addrword_t arg)
{
	while(1) {
		cyg_thread_suspend(threadhandle_3);

		/* do work */
		while (lds_in_wr != LDS_INPUT_SIZE) {

			/* if buffer is empty dont fetch new data */
			if (adc_rd == adc_wr && !adc_data_full)
				break;

			lds_input[lds_in_wr++] = adc_data[adc_rd];
			adc_rd = (adc_rd + 1) % ADC_DATA_SIZE;
		}

		if (lds_in_wr == LDS_INPUT_SIZE) { /* only process data if 64 values could be read (???) */
			lds_in_wr = 0;
			adc_data_full = false;

			// For Debugging
/*			int err = 0;
			for (i = 0; i < LDS_INPUT_SIZE; i++) {
				if (lds_input[i] != exp) {
			//		ezs_printf("error at %u: exp %f, got %f\n", (adc_rd + i) % ADC_DATA_SIZE, exp, lds_input[i]);
					err++;
				}
				exp = lds_input[i] + 1;
			}
            if (err != 0)
                ezs_printf("err: %d\n", err);

            ezs_printf("........\n");
			if (lds_input[0] != exp)
				ezs_printf("err1\n");
			if (lds_input[LDS_INPUT_SIZE - 1] != exp + LDS_INPUT_SIZE - 1)
				ezs_printf("err2\n");
			exp += LDS_INPUT_SIZE;
*/
			/* calculate output */
			if (lds_full)		//?????
				continue;
			float *lds_out = &(lds_output[lds_wr * LDS_INPUT_SIZE]);
			ezs_power_density_spectrum(lds_input, lds_out, LDS_INPUT_SIZE);
			lds_wr = (lds_wr + 1) % LDS_NUM;
			if (lds_rd == lds_wr)
			{
				//ezs_printf("full\n");
				lds_full = true;
			}
		}
	}
}


#define P_MIN (-140)
#define P_MAX 0
#define BLOCK_HEIGHT (CYG_FB_HEIGHT(FRAMEBUF) / (P_MAX - P_MIN))
#define BLOCK_WIDTH (CYG_FB_WIDTH(FRAMEBUF) / 32)
#define X_SIZE (BLOCK_WIDTH - (BLOCK_WIDTH / 8))
#define Y_POS (CYG_FB_HEIGHT(FRAMEBUF) - 1)

void thread_4(cyg_addrword_t arg)
{
	while(1) {
		cyg_thread_suspend(threadhandle_4);

		if (lds_rd == lds_wr && !lds_full)	// buffer empty -> is this needed???
			continue;

		uint32_t i;
		float *fb_input = &(lds_output[lds_rd * LDS_INPUT_SIZE]);

		/* write to framebuffer */
		ezs_fb_clear(0);
		int x_pos = 0;
	
 		for (i = 0; i < LDS_OUTPUT_SIZE; i++)
		{
			int y_size = 0 - (fb_input[i] - P_MIN) * BLOCK_HEIGHT;
			ezs_fb_fill_block(x_pos, Y_POS, X_SIZE, y_size, 3);
			x_pos += BLOCK_WIDTH;
		}

		lds_full = false;
		lds_rd = (lds_rd + 1) % LDS_NUM;
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
	cyg_clock_to_counter(cyg_real_time_clock(), &counter);


	/* create threads */
	cyg_thread_create(2, thread_1, 0, "Thread_1", stack_1, STACKSIZE, &threadhandle_1, &threaddata_1);
	cyg_thread_create(1, thread_2, 0, "Thread_2", stack_2, STACKSIZE, &threadhandle_2, &threaddata_2);
	cyg_thread_create(3, thread_3, 0, "Thread_3", stack_3, STACKSIZE, &threadhandle_3, &threaddata_3);
	cyg_thread_create(4, thread_4, 0, "Thread_4", stack_4, STACKSIZE, &threadhandle_4, &threaddata_4);

	/* create alarms */
	// reicht ein counter fuer alle alarme???	
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhandle_1, &alarmhandle_1, &alarm_1);
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhandle_2, &alarmhandle_2, &alarm_2);
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhandle_3, &alarmhandle_3, &alarm_3);
	cyg_alarm_create(counter, alarm_handler, (cyg_addrword_t) &threadhandle_4, &alarmhandle_4, &alarm_4);

	/* initialize alarms */
	cyg_alarm_initialize(alarmhandle_1, cyg_current_time() + 1, ms_to_cyg_ticks(16));//16));
	cyg_alarm_initialize(alarmhandle_2, cyg_current_time() + 1, ms_to_cyg_ticks(4));//4));
	cyg_alarm_initialize(alarmhandle_3, cyg_current_time() + 1, ms_to_cyg_ticks(256));//256));
	cyg_alarm_initialize(alarmhandle_4, cyg_current_time() + 1, ms_to_cyg_ticks(256));//256));

	/* enable alarms */
	cyg_alarm_enable(alarmhandle_1);
	cyg_alarm_enable(alarmhandle_2);
	cyg_alarm_enable(alarmhandle_3);
	cyg_alarm_enable(alarmhandle_4);
}

