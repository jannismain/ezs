#include <sml/ttkernel/c_api.h>
#include <pkgconf/ttkernel.h>
#include <stdio.h>
#include <inttypes.h>
#include <cyg/infra/diag.h>
#include "ezs_io.h"
#include "ezs_counter.h"
#include "ezs_trace.h"
#include "ezs_stopwatch.h"

//A little helper function
tt_ticktype ms_to_cyg_ticks(cyg_uint32 ms)
{
	cyg_resolution_t res = ttEcos_get_resolution();
	cyg_uint64 ticks = 1000000 * ms * (uint64_t)res.divisor / res.dividend;
	return (tt_ticktype)ticks;
}

tt_ticktype ms_to_ezs_ticks(cyg_uint32 ms)
{
	cyg_resolution_t res = ezs_counter_get_resolution();
	cyg_uint64 ticks = 1000000 * ms * (uint64_t)res.divisor / res.dividend;
	return (tt_ticktype)ticks;
}

// Declare Dispatcher table
tt_DispatcherTable(table1, 10);

// Declare task t1
tt_Task(t1)
{
	ezs_lose_time(ms_to_ezs_ticks(2), 100);
}

tt_Task(t2)
{
	ezs_lose_time(ms_to_ezs_ticks(2), 100);
}

tt_Task(t3)
{
    ezs_lose_time(ms_to_ezs_ticks(3), 100);
}

tt_Task(t4)
{
	ezs_lose_time(ms_to_ezs_ticks(6), 100);
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
	tt_InitTask(t2, TT_STRINGENT);
	tt_InitTask(t3, TT_STRINGENT);
	tt_InitTask(t4, TT_STRINGENT);

	// Initialize dispatcher table
	tt_InitDispatcherTable(table1);
	// Don't forget to test if table is valid
	bool valid = 1;
	/**
	 * BEWARE! Neiter ms_to_ezs_ticks nor ms_to_cyg_ticks are working!
	 * Fix them in order to complete this exercise
	 */
	// Insert entries:
	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(1) , TT_START_TASK, t1 );
	if (!valid) ezs_printf("1");
	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(10) , TT_DEADLINE  , t1 );
	if (!valid) ezs_printf("2");

	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(3) , TT_START_TASK, t2 );
	if (!valid) ezs_printf("1");
	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(22) , TT_DEADLINE  , t2 );
	if (!valid) ezs_printf("2");

	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(7), TT_START_TASK, t3 );
	if (!valid) ezs_printf("1");
	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(26) , TT_DEADLINE  , t3 );
	if (!valid) ezs_printf("2");
	
	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(14) , TT_START_TASK, t4 );
	if (!valid) ezs_printf("1");
	valid &= tt_DispatcherTableEntry( table1, ms_to_cyg_ticks(113) , TT_DEADLINE  , t4 );
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
