#include <sml/ttkernel/c_api.h>
#include <pkgconf/ttkernel.h>
#include <stdio.h>
#include <inttypes.h>

#include "ezs_io.h"
#include "ezs_counter.h"
#include "ezs_trace.h"
#include "ezs_stopwatch.h"

//A little helper function
tt_ticktype ms_to_cyg_ticks(cyg_uint32 ms)
{
	/**
	 * Implement if needed!
	 **/
	return ms;
}

tt_ticktype ms_to_ezs_ticks(cyg_uint32 ms)
{
	/**
	 * Implement if needed!
	 **/
	return ms;
}

// Declare Dispatcher table
tt_DispatcherTable(table1, 10);

// Declare task t1
tt_Task(t1)
{
	ezs_lose_time(ms_to_ezs_ticks(2), 0);
	ezs_printf("Task1\n");
}


// Idle task does nothing...
ttIdleTask
{
	while(1);
}


externC void cyg_user_start()
{
	// Initialize counter
	ezs_counter_init();
	
	// Initialize tasks with stringent deadline checking
	tt_InitTask(t1, TT_STRINGENT);

	// Initialize dispatcher table
	tt_InitDispatcherTable(table1);
	// Don't forget to test if table is valid
	bool valid = 1;
	/**
	 * BEWARE! Neiter ms_to_ezs_ticks nor ms_to_cyg_ticks are working!
	 * Fix them in order to complete this exercise
	 */
	// Insert entries:
	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(0) , TT_START_TASK, t1 );
	if (!valid) ezs_printf("1");
	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(9) , TT_DEADLINE  , t1 );
	if (!valid) ezs_printf("2");


	if(!valid)
	{
		ezs_printf("WARNING: Table not valid!\n");
	}
	else
	{
		ttStartOS(table1);
	}
}
