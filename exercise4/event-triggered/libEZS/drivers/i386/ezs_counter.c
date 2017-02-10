#define EZS_HPET_ADDR  0xFED00000

#include "ezs_counter.h"
#include <stdio.h>
#include <stdint.h>

typedef struct hpetReg {
  uint64_t  GCAP_ID;    /* General capabilities */
  uint64_t  rsv1;
  uint64_t  GEN_CONF;   /* General configuration */
  uint64_t  rsv2;
  uint64_t  GINTR_STA;    /* General Interrupt status */
  uint64_t  rsv3[25];
  uint64_t  MAIN_CNT;   /* Main counter */
  uint64_t  rsv4;
  uint64_t  TIM0_CONF;    /* Timer 0 config and cap */
#define     TIM_CONF 0
#define     Tn_INT_ENB_CNF 4
  uint64_t  TIM0_COMP;    /* Timer 0 comparator */
#define     TIM_COMP 8
  uint64_t  rsv5[2];
  uint64_t  TIM1_CONF;    /* Timer 1 config and cap */
  uint64_t  TIM1_COMP;    /* Timer 1 comparator */
  uint64_t  rsv6[2];
  uint64_t  TIM2_CONF;    /* Timer 2 config and cap */
  uint64_t  TIM2_COMP;    /* Timer 2 comparator */
  uint64_t  rsv7[2];
} hpetReg;
typedef struct  hpetReg hpetReg_t;

volatile hpetReg_t *hpet = (volatile hpetReg_t *) EZS_HPET_ADDR;

static uint64_t hpet_divisor = 0;
static uint32_t hpet_period = 0;
static bool use_hpet = true;


uint32_t ezs_hpet_period_fs(void)
{
  return hpet->GCAP_ID >> 32;
}

bool ezs_hpet_init(void)
{
  uint64_t test = hpet->GCAP_ID;
  uint64_t gcap = hpet->GCAP_ID;

  hpet_period = (gcap & 0xffffffff00000000ULL) >> 32;

  if( hpet_period > 0x5f5e100 ){
    use_hpet = false;
    printf("HPET invalid, using Time Stamp Counter\n");
    return false;
  } else {
    use_hpet = true;
  }

  uint8_t rev_id = gcap & 0xFF;
  uint8_t num_timers = (gcap >> 8) & 0x1F;
  uint8_t counter_size = (gcap >> 13) & 0x1;
  uint16_t vendor_id = (gcap >> 16) & 0xFFFF;

  hpet_divisor = 1000000000ULL / hpet_period; // 10e9 fs == 1 us

  printf("HPET enabled\n");
  /* enable HPET */
  hpet->GEN_CONF = 0x1;
}

uint64_t ezs_hpet_read(void)
{
  uint64_t cnt = hpet->MAIN_CNT;
  //printf("CNT: 0x%llx\n", cnt);
  return cnt;
}

uint64_t ezs_hpet_resolution_us(void)
{
  return 1;
}

uint64_t ezs_hpet_read_us(void)
{
  return ezs_hpet_read() / hpet_divisor;
}

/* Timestamp counter implementation */

#ifndef BOCHS_CPU_MHZ
#error Bochs CPU Speed not defined
#endif

static inline unsigned long long getticks(void)
{
  unsigned int lo, hi;
  // RDTSC copies contents of 64-bit TSC into EDX:EAX
  asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
  return (unsigned long long)hi << 32 | lo;
}

cyg_uint64 ezs_rdtsc_get_us(void) {
  return getticks() / (BOCHS_CPU_MHZ);
}

void ezs_tsc_init(void){
  printf("Using time stamp counter, cpu running @ %d MHz\n", BOCHS_CPU_MHZ);
}

static volatile uint32_t FAILBOCHSID = 0;

/* EZS API implementation */
void ezs_counter_init(void)
{
  if (FAILBOCHSID == 42) { // We run in Fail*
    ezs_tsc_init();
    use_hpet = false;
  } else { // not in Fail* try using HPET
    if ( ! ezs_hpet_init() ) { // HPET not accurate enough fall back to TSC
      ezs_tsc_init();
      use_hpet = false;
    }
  }
}


cyg_uint64 ezs_counter_get(void)
{
  if(use_hpet){
    return ezs_hpet_read_us();
  } else {
    return ezs_rdtsc_get_us();
  }
}

cyg_uint64 ezs_counter_resolution_us(void)
{
  return ezs_hpet_resolution_us();
}

cyg_uint64 ezs_counter_resolution_ps(void)
{
  return ezs_hpet_resolution_us() * 1000000;
}

