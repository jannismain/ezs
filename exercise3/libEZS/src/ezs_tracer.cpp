#include <pkgconf/system.h>
#include <cyg/io/io.h>
#include <cyg/error/codes.h>
#include <stdint.h>
#include <stdio.h>

#define TASKLOG_LEN 256
static volatile struct { cyg_uint32 timestamp, cyg_uint32 id} tasklog[TASKLOG_LEN];
static volatile int tasklog_index = 0;

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
			printf("time\tthreadid\n");
			for (i=0; i<TASKLOG_LEN; i++)
				printf("%i\t%i\n", tasklog[i].timestamp, tasklog[i].id);
			printf("end of tasklog\n");
		}
	}
}

