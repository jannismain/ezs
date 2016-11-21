#include "ezs_stopwatch.h"
#include "ezs_counter.h"
#include <stdlib.h>

void ezs_watch_start(cyg_uint32* state)
{
	*state = ezs_counter_get();
}

cyg_uint32 ezs_watch_stop(cyg_uint32* state)
{
	return  ezs_counter_get() - (*state);
}

void ezs_lose_time(cyg_uint32 wcet,  cyg_uint8 percentage)
{
	/**
	 * TODO: Do not implement!
	 * This function is not needed here. If you think otherwise:
	 *   Use YOUR implementation from assignment "Antwortzeit".
	 */
	return;
}


