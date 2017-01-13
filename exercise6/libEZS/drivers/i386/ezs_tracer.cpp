#include <pkgconf/system.h>
#include <cyg/io/io.h>
#include <cyg/error/codes.h>
#include <stdint.h>
#include <stdio.h>

//! The magic tracer variable Fail* is listening on.
volatile unsigned ezs_tracer_register;

//! No Time triggered kernel -> normal event triggered ecos:
#ifndef SMLPKG_TTKERNEL
#include <cyg/kernel/kernel.hxx> 	// C Kernel-API
extern "C" {
#include <cyg/kernel/kapi.h> 	// C Kernel-API
}
class Cyg_Thread;


extern "C" void ezs_instrument(Cyg_Thread* current, Cyg_Thread* next) {
    // Get priority from next thread
	int prio = next->get_priority();
	if(prio > 32) prio = 32;

    // Write priority to tracer register
    ezs_tracer_register = prio;
}

#else
#include <sml/ttkernel/task.hxx>
extern "C" void ezs_instrument(TT_Task* current, TT_Task* next){
    ezs_tracer_register = next->get_id();
}
#endif

