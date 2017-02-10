#include "ezs_counter.h"
#include "math.h"

#include <libopencm3/stm32/f4/memorymap.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <stdint.h>

extern "C" {

static cyg_resolution_t resolution;
static cyg_uint32 res_ps;

#define PRESCALER 0
#define RCCCLOCK 42 /*MHz*/

// CPU SPEED is 84MHz
// RCC CLOCK is CPUSPEED / 2 = 42MHz
// timer increment frequency is RCC CLOCK / (PRESCALER+1)
// counter will overflow after (PRESCALER+1) * 102sec

void ezs_counter_init(void) {
	rcc_periph_clock_enable(RCC_TIM5);

	timer_set_mode(TIM5, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_continuous_mode(TIM5);
	timer_set_period(TIM5, 0xFFFFFFFF);
	timer_set_prescaler(TIM5, PRESCALER);
	timer_enable_preload(TIM5);

	timer_enable_counter(TIM5);
	
	resolution.dividend = 1000L*(PRESCALER+1);
	resolution.divisor =  RCCCLOCK;
	res_ps = (PRESCALER+1) * 1000000L / RCCCLOCK;
}

cyg_uint32 ezs_counter_get(void) {
	return timer_get_counter(TIM5);
}

cyg_resolution_t ezs_counter_get_resolution(void){
	return resolution;
}

cyg_uint32 ezs_counter_resolution_ps(void) {
	return res_ps;
}

}
