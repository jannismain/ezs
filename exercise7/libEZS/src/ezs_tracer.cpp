#include <pkgconf/system.h>
#include <cyg/io/io.h>
#include <cyg/error/codes.h>
#include <stdint.h>
#include <stdio.h>
#include "ezs_counter.h"
#include "ezs_io_fel.h"

#ifndef TASKLOG_LEN
// Providing a default
#define TASKLOG_LEN 256
#endif
static volatile struct { cyg_uint32 timestamp; cyg_uint32 id;} tasklog[TASKLOG_LEN];
volatile int tasklog_index = TASKLOG_LEN;

#define TRACE_FILE "tracefile"
//! No Time triggered kernel -> normal event triggered ecos:
#ifndef SMLPKG_TTKERNEL
#include <cyg/kernel/kernel.hxx> // C Kernel-API
extern "C" {
#include <cyg/kernel/kapi.h> // C Kernel-API
}
class Cyg_Thread;


extern "C" void ezs_instrument(Cyg_Thread* current, Cyg_Thread* next) {
	int id = next->get_priority(); // abuse the priority (which must be unique) as sort-of thread-id
#else
#include <sml/ttkernel/task.hxx>
extern "C" void ezs_instrument(TT_Task* current, TT_Task* next){
	int id = next->get_id();
#endif
	if (tasklog_index < TASKLOG_LEN)
	{
		tasklog[tasklog_index].timestamp = ezs_counter_get();
		tasklog[tasklog_index].id = id;
		tasklog_index++;

		if (tasklog_index == TASKLOG_LEN)
		{
			int i;
			ezs_log_file(TRACE_FILE, "# time\tthreadid\n");
			for (i=0; i<TASKLOG_LEN; i++)
				ezs_log_file(TRACE_FILE, "%i\t%i\n", tasklog[i].timestamp, tasklog[i].id);
			ezs_log_file(TRACE_FILE, "# end of tasklog\n");
		}
	}
}

