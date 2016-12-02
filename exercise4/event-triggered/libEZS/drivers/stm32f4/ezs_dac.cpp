/*!
 * @file ezs_dac.c
 * @brief A C wrapper for the stm32 pwm driver
 * @author Florian Jung
 * \ingroup dac 
 */

#define EZS_PWM  CYGHWR_HAL_STM32_PIN_ALTFN_OUT(D,15, 2, PUSHPULL,NONE,LOW)

#include <cyg/hal/hal_arch.h>

#include <libopencm3/stm32/f4/memorymap.h>
#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/f4/rcc.h>
#include <libopencm3/stm32/f4/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <stdint.h>


/// The timer ticks per microsecond.  Can be adjusting by measuring
/// pwm on o-scope
#define PWM_TIMER_TICKS_PER_MICROSECOND 84.0

/// PWM Frequency, default is 62 kHz
#define PWM_FREQUENCY_kHz 162.0

/// PWM Period, set automatically by the options above
#define PWM_PERIOD ((1.0/PWM_FREQUENCY_kHz) * 1000.0 \
                    * PWM_TIMER_TICKS_PER_MICROSECOND / 2)


/*
                                --- FTFM (with edits) ---
Bullet points are the steps needed.
Pulse width modulation mode allows you to generate a signal with a frequency determined
by the value of the TIMx_ARR register (the period) and a duty cycle determined by the value of the
TIMx_CCRx register (the output compare value).
  -- Set TIMx_ARR to desired frequency
  -- Set TIMx_CCRx to desired duty cycle
The PWM mode can be selected independently on each channel (one PWM per OCx
output) by writing 110 (PWM mode 1) or ‘111 (PWM mode 2) in the OCxM bits in the
TIMx_CCMRx register.
  -- Write PWM Mode 1 or PWM Mode 2 to OCxM bits in TIMx_CCMRx register
You must enable the corresponding preload register by setting the
OCxPE bit in the TIMx_CCMRx register, and eventually the auto-reload preload register by
setting the ARPE bit in the TIMx_CR1 register.
  -- Set corresponding OCxPE bit in TIMx_CCMRx register
  -- Set ARPE bit in TIMx_CR1
As the preload registers are transferred to the shadow registers only when an update event
occurs, before starting the counter, you have to initialize all the registers by setting the UG
bit in the TIMx_EGR register.
  -- set UG bit in TIMx_EGR register
OCx polarity is software programmable using the CCxP bit in the TIMx_CCER register. It
can be programmed as active high or active low. OCx output is enabled by the CCxE bit in
the TIMx_CCER register. Refer to the TIMx_CCERx register description for more details.
  -- set desired polarity in TIMx_CCER
  -- set CCxE bit in TIMx_CCER  (enable output)
*/
static void pwm_init(uint32_t timer, uint8_t channel, uint32_t period) {
// function stolen from somewhere on stackoverflow

  // Convert channel number to internal rep
  enum tim_oc_id chan;
  switch (channel) {
    case 1:   chan = TIM_OC1; break;
    case 2:   chan = TIM_OC2; break;
    case 3:   chan = TIM_OC3; break;
    case 4:   chan = TIM_OC4; break;
    default: chan = TIM_OC4; break;
  }

  // Timer Base Configuration
  // timer_reset(timer);
  timer_set_mode(timer, TIM_CR1_CKD_CK_INT, // clock division
                        TIM_CR1_CMS_EDGE,   // Center-aligned mode selection
                        TIM_CR1_DIR_UP);    // TIMx_CR1 DIR: Direction
  timer_continuous_mode(timer);             // Disables TIM_CR1_OPM (One pulse mode)
  timer_set_period(timer, period);                    // Sets TIMx_ARR
  timer_set_prescaler(timer, 1);               // Adjusts speed of timer
  timer_set_clock_division(timer, 0);            // Adjusts speed of timer
  timer_set_master_mode(timer, TIM_CR2_MMS_UPDATE);   // Master Mode Selection
  timer_enable_preload(timer);                        // Set ARPE bit in TIMx_CR1

  // Channel-specific settings
  timer_set_oc_value(timer, chan, 0);             // sets TIMx_CCRx
  timer_set_oc_mode(timer, chan, TIM_OCM_PWM1);   // Sets PWM Mode 1
  timer_enable_oc_preload(timer, chan);           // Sets OCxPE in TIMx_CCMRx
  timer_set_oc_polarity_high(timer, chan);        // set desired polarity in TIMx_CCER
  timer_enable_oc_output(timer, chan);             // set CCxE bit in TIMx_CCER  (enable output)

  // Initialize all counters in the register
  switch (timer) {
    case TIM1:  TIM1_EGR |= TIM_EGR_UG; break;
    case TIM2:  TIM2_EGR |= TIM_EGR_UG; break;
    case TIM3:  TIM3_EGR |= TIM_EGR_UG; break;
    case TIM4:  TIM4_EGR |= TIM_EGR_UG; break;
    case TIM5:  TIM5_EGR |= TIM_EGR_UG; break;
    case TIM6:  TIM6_EGR |= TIM_EGR_UG; break;
    case TIM7:  TIM7_EGR |= TIM_EGR_UG; break;
    case TIM8:  TIM8_EGR |= TIM_EGR_UG; break;
    }
}

static void pwm_setup(void) {
  rcc_periph_clock_enable(RCC_TIM4);
  //pwm_init(TIM4, 1, PWM_PERIOD);
  pwm_init(TIM4, 2, PWM_PERIOD);
  pwm_init(TIM4, 3, PWM_PERIOD);
  pwm_init(TIM4, 4, PWM_PERIOD);

  // LED channels = PD12..15
  rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPDEN);

  gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE,
                  GPIO13 | GPIO14 | GPIO15);
  // AF2 = TIM4_CH1..4
  gpio_set_af(GPIOD, GPIO_AF2, GPIO13 | GPIO14 | GPIO15);
  timer_enable_counter(TIM4);
}

extern "C" void ezs_dac_init() {
	pwm_setup();
	CYGHWR_HAL_STM32_GPIO_SET(EZS_PWM);
}

extern "C" void ezs_dac_write(uint8_t value) {
	timer_set_oc_value(TIM4, TIM_OC2, value*PWM_PERIOD / 256);
	timer_set_oc_value(TIM4, TIM_OC3, value*PWM_PERIOD / 256);
	timer_set_oc_value(TIM4, TIM_OC4, value*PWM_PERIOD / 256);
}

