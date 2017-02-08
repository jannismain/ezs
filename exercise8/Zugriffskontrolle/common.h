#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <cyg/hal/hal_arch.h>

#define STACKSIZE    (CYGNUM_HAL_STACK_SIZE_MINIMUM+4096)


cyg_handle_t s_real_time_counter;
cyg_addrword_t data_dummy;

void init_tasks(void); // defined in aufgabe_X.c
void lose_time_us(int ms, int percentage); // defined in app.c

#endif // COMMON_H_INCLUDED
