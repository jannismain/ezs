#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>

#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "ezs_counter.h"
#include "ezs_serial.h"
#include "ezs_sensor.h"
#include "ezs_stopwatch.h"
#include "ezs_adc.h"
#include "ezs_dac.h"
#include "ezs_io_fel.h"
#include "ezs_fft.h"
#include "ezs_interpolation.h"
#include "ezs_plot.h"
#include "ezs_plot_pds.h"

#define SERIAL_IRQ CYGNUM_HAL_INTERRUPT_UART2

/*
 * TODO: Code anpassen
 */
#define SAMPLING_TASK_PRIORITY 10
#define EDGE_TASK_PRIORITY 11
#define DISPLAY_SIGNAL_TASK_PRIORITY 14
#define DISPLAY_PDS_TASK_PRIORITY 16
#define ANALYSIS_TASK_PRIORITY 15

#define DECODING_TASK_PRIORITY 20 
#define POLLING_TASK_PRIORITY 21
#define STATE_MACHINE_TASK_PRIORITY 22
#define TRIGGER_TASK_PRIORITY 16

#define SAMPLING_TASK_PERIOD 4
#define EDGE_TASK_PERIOD 4
#define DISPLAY_SIGNAL_TASK_PERIOD 250
#define DISPLAY_PDS_TASK_PERIOD 1000
#define ANALYSIS_TASK_PERIOD 1000
#define POLLING_TASK_PERIOD 50
#define STATE_MACHINE_TASK_PERIOD 50
#define TRIGGER_TASK_PERIOD 250

#define SAMPLING_TASK_PHASE 0
#define EDGE_TASK_PHASE 1
#define ANALYSIS_TASK_PHASE 0
#define DISPLAY_SIGNAL_TASK_PHASE 0
#define DISPLAY_PDS_TASK_PHASE 140
#define POLLING_TASK_PHASE 600
#define STATE_MACHINE_TASK_PHASE 650
#define TRIGGER_TASK_PHASE 0

#define STACKSIZE    (CYGNUM_HAL_STACK_SIZE_MINIMUM+4096)

#define PDS_LENGTH 32
#define TIME_DOMAIN_LENGTH (2 * PDS_LENGTH)
static cyg_uint32 s_time_domain[TIME_DOMAIN_LENGTH];
static float s_frequency_domain[PDS_LENGTH];
//static unsigned int s_position = 0;
static unsigned int time_write = 0;
static unsigned int time_read_3 = 0;
static unsigned int time_read_4 = 0;

#define SERIAL_BUFFER_LENGTH 15
static volatile char c;
static volatile bool c_empty = true;
static char s_serial_buffer[SERIAL_BUFFER_LENGTH];
static unsigned int s_serial_position = 0;
static bool s_command_decodable = false;

static cyg_handle_t sampling_task_alarm_handle;
static cyg_handle_t edge_task_alarm_handle;
static cyg_handle_t analysis_task_alarm_handle;
static cyg_handle_t display_signal_task_alarm_handle;
static cyg_handle_t display_pds_task_alarm_handle;
static cyg_handle_t polling_task_alarm_handle;
static cyg_handle_t statemachine_task_alarm_handle;
static cyg_handle_t trigger_task_alarm_handle;


static cyg_alarm sampling_task_alarm;
static cyg_alarm edge_task_alarm;
static cyg_alarm analysis_task_alarm;
static cyg_alarm display_signal_task_alarm;
static cyg_alarm display_pds_task_alarm;
static cyg_alarm polling_task_alarm;
static cyg_alarm statemachine_task_alarm;
static cyg_alarm trigger_task_alarm;

//FLAG t1, t2
#define SIGNAL_STATE_TRIGGER    (1 << 1)
static cyg_flag_t flag;
static cyg_uint32 flag_value = 0;


//Mailbox t2, t9
static cyg_handle_t mailbox_handle;
static cyg_mbox mailbox;

enum CommandStatus
{
	CommandComplete,
	CommandIncomplete
};

static enum CommandStatus packet_receive(char c)
{
	assert(s_serial_position < SERIAL_BUFFER_LENGTH);
    if (c == '\n') {
        s_serial_buffer[s_serial_position] = '\0';
        s_serial_position = 0;
        s_command_decodable = true;
        return CommandComplete;
    }

    s_serial_buffer[s_serial_position++] = c;
    return CommandIncomplete;
}

enum Command
{
	DisplayTime = (1 << 1),
	DisplayPDS  = (1 << 2),
	TriggerOn   = (1 << 3),
	TriggerOff  = (1 << 4),
	TLevelRise  = (1 << 5),
	TLevelFall  = (1 << 6),
	Invalid     = 0x00,
};

enum State
{
	StateDisplayTime    = (1 << 1),
	StateTriggerOn      = (1 << 2),
	StateTLevelRise     = (1 << 3),
};
//Little helper functions.
static cyg_tick_count_t ms_to_cyg_ticks(cyg_uint32 ms)
{
	cyg_handle_t clock = cyg_real_time_clock();
	cyg_resolution_t res = cyg_clock_get_resolution(clock);
	cyg_uint64 ticks = 1000000 * ms * (uint64_t)res.divisor / res.dividend;
	return (cyg_tick_count_t)ticks;
}

static cyg_tick_count_t ms_to_ezs_ticks(cyg_uint32 ms) {
	cyg_resolution_t res = ezs_counter_get_resolution();
	cyg_uint64 ticks = 1000000 * ms * (uint64_t)res.divisor / res.dividend;
	return (cyg_tick_count_t)ticks;
}

static cyg_uint64 ezs_ticks_to_ps(cyg_uint32 ticks) {
	cyg_resolution_t res = ezs_counter_get_resolution();
    cyg_uint64 ms = ticks;
    ms *= res.dividend;
    ms /= res.divisor;
    return ms;
}

enum Command decode_command(void)
{
	enum Command ret = Invalid;
    if (!s_command_decodable)
        return ret;
    if (!strncmp(s_serial_buffer, "display signal", 15)) {
        ret = DisplayTime;
    } else if (!strncmp(s_serial_buffer, "display pds", 15)) {
        ret = DisplayPDS;
    } else if (!strncmp(s_serial_buffer, "trigger off", 15)) {
        ret = TriggerOff;
    } else if (!strncmp(s_serial_buffer, "trigger on", 15)) {
        ret = TriggerOn;
    } else if (!strncmp(s_serial_buffer, "tlevel rise", 15)) {
        ret = TLevelRise;
    } else if (!strncmp(s_serial_buffer, "tlevel fall", 15)) {
        ret = TLevelFall;
    }
    s_command_decodable = false;
	return ret;
}

cyg_uint32 serial_isr_handler(cyg_vector_t vector, cyg_addrword_t data)
{
    if (!c_empty) {
		cyg_interrupt_acknowledge(vector);
        return CYG_ISR_HANDLED;
    }

	if (ezs_serial_char_available())
	{
	    c = ezs_serial_getc();
        c_empty = false;
		cyg_interrupt_acknowledge(vector);
		return CYG_ISR_CALL_DSR;
	}
	else
	{
		cyg_interrupt_acknowledge(vector);
		return CYG_ISR_HANDLED;
	}

}

enum CommandStatus status = CommandIncomplete;
void serial_dsr_handler(cyg_vector_t vec, cyg_ucount32 count, cyg_addrword_t data)
{
    status = packet_receive(c);
    c_empty = true;
}


enum Command cmd = Invalid;
// T7
static cyg_uint8     polling_task_stack[STACKSIZE];
static cyg_handle_t  polling_task_handle;
static cyg_thread    polling_task_thread;
static void polling_task_entry(cyg_addrword_t data)
{
	while (1)
	{
        if (status == CommandComplete) {
            cmd = decode_command();
            status = CommandIncomplete; 
        }
        cyg_thread_suspend(cyg_thread_self());
	}
}

// Zustandsmaschine
enum State state = StateDisplayTime;
static cyg_uint8 trigger_high = 0;
static cyg_uint8 trigger_low = 1;

// Helper function for statemachine
static void switch_to_time_mode(void)
{
    flag_value &= ~(SIGNAL_STATE_TRIGGER);

    // disable: T2, T4, T5, T9
    cyg_alarm_disable(edge_task_alarm_handle);
    cyg_alarm_disable(analysis_task_alarm_handle);
    cyg_alarm_disable(display_pds_task_alarm_handle);
    cyg_alarm_disable(trigger_task_alarm_handle);

    // enable:  T3
    cyg_alarm_enable(display_signal_task_alarm_handle);
}

static void switch_to_pds_mode(void)
{
    flag_value &= ~(SIGNAL_STATE_TRIGGER);

    // disable: T2, T3, T9
    cyg_alarm_disable(edge_task_alarm_handle);
    cyg_alarm_disable(display_signal_task_alarm_handle);
    cyg_alarm_disable(trigger_task_alarm_handle);

    // enable:  T4, T5
    cyg_alarm_enable(analysis_task_alarm_handle);
    cyg_alarm_enable(display_pds_task_alarm_handle);
}

static void switch_to_trigger_mode(void)
{
    flag_value |= SIGNAL_STATE_TRIGGER;

    // disable: T3, T4, T5
    cyg_alarm_disable(display_signal_task_alarm_handle);
    cyg_alarm_disable(analysis_task_alarm_handle);
    cyg_alarm_disable(display_pds_task_alarm_handle);

    // enable:  T2, T9
    cyg_alarm_enable(edge_task_alarm_handle);
    cyg_alarm_enable(trigger_task_alarm_handle);
}

// T8
static cyg_uint8     statemachine_task_stack[STACKSIZE];
static cyg_handle_t  statemachine_task_handle;
static cyg_thread    statemachine_task_thread;
static void statemachine_task_entry(cyg_addrword_t data)
{
	while (1)
	{
        switch (cmd) {
            case (DisplayTime):
            {
                if (state & StateDisplayTime)
                    break;
                state |= StateDisplayTime;

                if (state & StateTriggerOn) {
                    switch_to_trigger_mode();
                } else {
                    switch_to_time_mode();
                }

                break;
            }             
            case (DisplayPDS):
            {
                if (!(state & StateDisplayTime))
                    break;
                state &= ~(StateDisplayTime);

                switch_to_pds_mode();

                break;
            }
            case (TriggerOn):
            {
                if (state & StateTriggerOn)
                    break;
                state |= StateTriggerOn;

                if (state & StateDisplayTime)
                    switch_to_trigger_mode();

                break;
            }
            case (TriggerOff):
            {
                if (!(state & StateTriggerOn))
                    break;
                state &= ~(StateTriggerOn);

                if (state & StateDisplayTime)
                    switch_to_time_mode();

                break;
            }
            case (TLevelRise):
            {
                if (state & StateTLevelRise)
                    break;
                state |= StateTLevelRise;
                break;
            }
            case (TLevelFall):
            {
                if (!(state & StateTLevelRise))
                    break;
                state &= ~(StateTLevelRise);
                break;
            }
        }

        cmd = Invalid;

        cyg_thread_suspend(cyg_thread_self());
    }
}
//T9
static cyg_uint8     trigger_task_stack[STACKSIZE];
static cyg_handle_t  trigger_task_handle;
static cyg_thread    trigger_task_thread;
static void trigger_task_entry(cyg_addrword_t data)
{
    while (1)
    {
        cyg_uint32 *data = cyg_mbox_get(mailbox_handle);
        ezs_plot(data, TIME_DOMAIN_LENGTH, FB_BLACK, FB_WHITE);
        ezs_printf("trigger\n");
//        cyg_thread_suspend(cyg_thread_self());
    }
}


// T1
static cyg_uint8     sampling_task_stack[STACKSIZE];
static cyg_handle_t  sampling_task_handle;
static cyg_thread    sampling_task_thread;
static void sampling_task_entry(cyg_addrword_t data)
{
	while (1)
	{
        uint8_t x = ezs_adc_read();
        s_time_domain[time_write++] = x;
        time_write %= TIME_DOMAIN_LENGTH;
        cyg_flag_setbits(&flag, flag_value);
		cyg_thread_suspend(cyg_thread_self());
	}
}

// T2
static cyg_uint8     edge_task_stack[STACKSIZE];
static cyg_handle_t  edge_task_handle;
static cyg_thread    edge_task_thread;
static cyg_uint32 trigger_data[TIME_DOMAIN_LENGTH];
static void edge_task_entry(cyg_addrword_t data)
{
    static cyg_uint32 old_value = 188;
    static cyg_uint32 idx = 0;

	while (1)
	{
        cyg_flag_wait(&flag, SIGNAL_STATE_TRIGGER, CYG_FLAG_WAITMODE_OR | CYG_FLAG_WAITMODE_CLR);

        uint8_t value = s_time_domain[idx];

        if ( (old_value < 188 && value > 188 && (state & StateTLevelRise))          // rising edge
             || (old_value > 188 && value < 188 && !(state & StateTLevelRise)) ) {  // falling edge
            cyg_mbox_put(mailbox_handle, (void *)(trigger_data));
        }

        trigger_data[idx] = value;

        idx = (idx + 1) % TIME_DOMAIN_LENGTH;
        old_value = value;
//    	cyg_thread_suspend(cyg_thread_self());
	}
}

// T3
static cyg_uint32 time_domain_copy[TIME_DOMAIN_LENGTH];

static cyg_uint8     analysis_task_stack[STACKSIZE];
static cyg_handle_t  analysis_task_handle;
static cyg_thread    analysis_task_thread;
static void analysis_task_entry(cyg_addrword_t data)
{
	while (1)
	{
        int idx;
        for (idx = 0; idx < TIME_DOMAIN_LENGTH; idx++) {
            time_domain_copy[idx] = s_time_domain[time_read_3++];
            time_read_3 %= TIME_DOMAIN_LENGTH;
        }

        ezs_easy_pds(time_domain_copy, s_frequency_domain, TIME_DOMAIN_LENGTH);
		cyg_thread_suspend(cyg_thread_self());
	}
}


// T4
static cyg_uint8     display_signal_task_stack[STACKSIZE];
static cyg_handle_t  display_signal_task_handle;
static cyg_thread    display_signal_task_thread;
static void display_signal_task_entry(cyg_addrword_t data)
{
	while (1)
	{
        ezs_plot(s_time_domain, TIME_DOMAIN_LENGTH, FB_BLACK, FB_WHITE); 
		cyg_thread_suspend(cyg_thread_self());
	}
}

// T5
static cyg_uint8     display_pds_task_stack[STACKSIZE];
static cyg_handle_t  display_pds_task_handle;
static cyg_thread    display_pds_task_thread;
static void display_pds_task_entry(cyg_addrword_t data)
{
	while (1)
	{
        ezs_plot_pds(s_frequency_domain, PDS_LENGTH, FB_BLACK, FB_WHITE); 

		cyg_thread_suspend(cyg_thread_self());
	}
}

static void sampling_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(sampling_task_handle);
}

static void edge_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(edge_task_handle);
}

static void analysis_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(analysis_task_handle);
}

static void display_signal_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(display_signal_task_handle);
}

static void display_pds_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(display_pds_task_handle);
}

static void polling_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(polling_task_handle);
}

static void statemachine_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(statemachine_task_handle);
}

static void trigger_task_alarmfn(cyg_handle_t alarmH, cyg_addrword_t data)
{
	cyg_thread_resume(trigger_task_handle);
}


static cyg_handle_t real_time_counter;

static cyg_addrword_t data_dummy = 0;
static cyg_interrupt serial_intr;
static cyg_handle_t  serial_isr_handle;

void cyg_user_start(void)
{
	ezs_fb_init();
	ezs_counter_init();
	ezs_sensors_init();
	ezs_fel_serial_init();

    cyg_flag_init(&flag);
    cyg_mbox_create(&mailbox_handle, &mailbox);

	cyg_interrupt_create(SERIAL_IRQ,
	                     1,
	                     0,
	                     serial_isr_handler,
	                     serial_dsr_handler,
	                     &serial_isr_handle,
	                     &serial_intr) ;
	cyg_interrupt_attach(serial_isr_handle);
	cyg_interrupt_unmask(SERIAL_IRQ);

	cyg_thread_create(SAMPLING_TASK_PRIORITY, &sampling_task_entry, 0, "sampling task",
	                  sampling_task_stack, STACKSIZE,
	                  &sampling_task_handle, &sampling_task_thread);
	cyg_thread_create(EDGE_TASK_PRIORITY, &edge_task_entry, 0, "edge task",
	                  edge_task_stack, STACKSIZE,
	                  &edge_task_handle, &edge_task_thread);
	cyg_thread_create(ANALYSIS_TASK_PRIORITY, &analysis_task_entry, 0, "analysis task",
	                  analysis_task_stack, STACKSIZE,
	                  &analysis_task_handle, &analysis_task_thread);
	cyg_thread_create(DISPLAY_SIGNAL_TASK_PRIORITY, &display_signal_task_entry, 0, "display signal task",
	                  display_signal_task_stack, STACKSIZE,
	                  &display_signal_task_handle, &display_signal_task_thread);
	cyg_thread_create(DISPLAY_PDS_TASK_PRIORITY, &display_pds_task_entry, 0, "display pds task",
	                  display_pds_task_stack, STACKSIZE,
	                  &display_pds_task_handle, &display_pds_task_thread);
	cyg_thread_create(POLLING_TASK_PRIORITY, &polling_task_entry, 0, "polling task",
	                  polling_task_stack, STACKSIZE,
	                  &polling_task_handle, &polling_task_thread);
	cyg_thread_create(STATE_MACHINE_TASK_PRIORITY, &statemachine_task_entry, 0, "statemachine task",
	                  statemachine_task_stack, STACKSIZE,
	                  &statemachine_task_handle, &statemachine_task_thread);
	cyg_thread_create(TRIGGER_TASK_PRIORITY, &trigger_task_entry, 0, "trigger task",
	                  trigger_task_stack, STACKSIZE,
	                  &trigger_task_handle, &trigger_task_thread);
	
    cyg_clock_to_counter(cyg_real_time_clock(), &real_time_counter);
	cyg_uint32 timebase = cyg_current_time() + 3;

	cyg_alarm_create(real_time_counter, sampling_task_alarmfn, data_dummy, &sampling_task_alarm_handle, &sampling_task_alarm);
	cyg_alarm_initialize(sampling_task_alarm_handle, timebase + ms_to_cyg_ticks(SAMPLING_TASK_PHASE), ms_to_cyg_ticks(SAMPLING_TASK_PERIOD));
	cyg_alarm_create(real_time_counter, edge_task_alarmfn, data_dummy, &edge_task_alarm_handle, &edge_task_alarm);
	cyg_alarm_initialize(edge_task_alarm_handle, timebase + ms_to_cyg_ticks(EDGE_TASK_PHASE), ms_to_cyg_ticks(EDGE_TASK_PERIOD));
    cyg_alarm_create(real_time_counter, analysis_task_alarmfn, data_dummy, &analysis_task_alarm_handle, &analysis_task_alarm);
    cyg_alarm_initialize(analysis_task_alarm_handle, timebase + ms_to_cyg_ticks(ANALYSIS_TASK_PHASE), ms_to_cyg_ticks(ANALYSIS_TASK_PERIOD));
	cyg_alarm_create(real_time_counter, display_signal_task_alarmfn, data_dummy, &display_signal_task_alarm_handle, &display_signal_task_alarm);
	cyg_alarm_initialize(display_signal_task_alarm_handle, timebase + ms_to_cyg_ticks(DISPLAY_SIGNAL_TASK_PHASE), ms_to_cyg_ticks(DISPLAY_SIGNAL_TASK_PERIOD));
	cyg_alarm_create(real_time_counter, display_pds_task_alarmfn, data_dummy, &display_pds_task_alarm_handle, &display_pds_task_alarm);
	cyg_alarm_initialize(display_pds_task_alarm_handle, timebase + ms_to_cyg_ticks(DISPLAY_PDS_TASK_PHASE), ms_to_cyg_ticks(DISPLAY_PDS_TASK_PERIOD));
	cyg_alarm_create(real_time_counter, polling_task_alarmfn, data_dummy, &polling_task_alarm_handle, &polling_task_alarm);
	cyg_alarm_initialize(polling_task_alarm_handle, timebase + ms_to_cyg_ticks(POLLING_TASK_PHASE), ms_to_cyg_ticks(POLLING_TASK_PERIOD));

	cyg_alarm_create(real_time_counter, statemachine_task_alarmfn, data_dummy, &statemachine_task_alarm_handle, &statemachine_task_alarm);
	cyg_alarm_initialize(statemachine_task_alarm_handle, timebase + ms_to_cyg_ticks(STATE_MACHINE_TASK_PHASE), ms_to_cyg_ticks(STATE_MACHINE_TASK_PERIOD));

	cyg_alarm_create(real_time_counter, trigger_task_alarmfn, data_dummy, &trigger_task_alarm_handle, &trigger_task_alarm);
	cyg_alarm_initialize(trigger_task_alarm_handle, timebase + ms_to_cyg_ticks(TRIGGER_TASK_PHASE), ms_to_cyg_ticks(TRIGGER_TASK_PERIOD));

    cyg_alarm_disable(display_pds_task_alarm_handle);
    cyg_alarm_disable(analysis_task_alarm_handle);
    cyg_alarm_disable(edge_task_alarm_handle);
    cyg_alarm_disable(trigger_task_alarm_handle);
}
