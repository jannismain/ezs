#include <cyg/kernel/kapi.h>
#include <stdlib.h>

#include "ezs_delay.h"
#include "ezs_counter.h"

void ezs_delay_us(cyg_uint32 microseconds)
{
	cyg_uint32 start = ezs_counter_get();
	cyg_uint32 waitticks = ((cyg_uint64)microseconds) * 1000000L / ezs_counter_resolution_ps();

	if (microseconds >= 1000 /* 1ms */)
		cyg_thread_delay(microseconds / 1000);

	while (ezs_counter_get()-start < waitticks);
}

