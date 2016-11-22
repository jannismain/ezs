#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#include <stdio.h>
#include <iso646.h>
#include <stdint.h>
#include <stdlib.h>

#include "ezs_counter.h"
#include "ezs_delay.h"
#include "ezs_gpio.h"
#include "ezs_stopwatch.h"
#include "ezs_io.h"

#define STACKSIZE CYGNUM_HAL_STACK_SIZE_MINIMUM+102400
static cyg_uint8 my_stack[STACKSIZE];
static cyg_handle_t thread_handle;
static cyg_thread threaddata;

#define DATA_SIZE 512
static uint32_t g_data[DATA_SIZE];

void swap(uint32_t data[], size_t first, size_t second)
{
	uint32_t tmp = data[first];
	data[first] = data[second];
	data[second] = tmp;
}

void heapsort(uint32_t arr[], unsigned int N)
{
	int t;			/* the temporary value */
	unsigned int n = N, parent = N / 2, index, child;	/* heap indexes */
	/* loop until array is sorted */
	while (1)
	{
		if (parent > 0)
		{
			/* first stage - Sorting the heap */
			t = arr[--parent];	/* save old value to t */
		}
		else
		{
			/* second stage - Extracting elements in-place */
			--n;	/* make the heap smaller */
			if (n == 0)
			{
				return;	/* When the heap is empty, we are done */
			}
			t = arr[n];	/* save lost heap entry to temporary */
			arr[n] = arr[0];	/* save root entry beyond heap */
		}
		/* insert operation - pushing t down the heap to replace the parent */
		index = parent;	/* start at the parent index */
		child = index * 2 + 1;	/* get its left child index */
		while (child < n)
		{
			/* choose the largest child */
			if (child + 1 < n && arr[child + 1] > arr[child])
			{
				++child;	/* right child exists and is bigger */
			}
			/* is the largest child larger than the entry? */
			if (arr[child] > t)
			{
				arr[index] = arr[child];	/* overwrite entry with child */
				index = child;	/* move index to the child */
				child = index * 2 + 1;	/* get the left child and go around again */
			}
			else
			{
				break;	/* t's place is found */
			}
		}
		/* store the temporary value at its new location */
		arr[index] = t;
	}
}

void heapsort_job(void)
{
	heapsort(g_data, DATA_SIZE);
}

void bubblesort(uint32_t array[], unsigned int N) {
    cyg_uint32 time;
    ezs_watch_start(&time);

	int i, j;
	for (i = 0; i < N - 1; ++i) {
		for (j = 0; j < N - i - 1; ++j) {
			if (array[j] > array[j + 1]) {
				// Bubble up!
				swap(array, i, j);
			}
		}
	}
    ezs_watch_stop(&time);
    ezs_printf(" %u \n", (time*ezs_counter_resolution_ps()/1000000));
}

void bubblesort_test(void) {
	size_t i;
    cyg_uint32 j;
    cyg_uint32 k;

    for (k = 64; k < 550; k*=2) {
        ezs_printf("\nstart bubblesort with arraysize %u\n\n", k);
        /* values already sorted */
        for (i = 0; i < k; ++i)
        {
            g_data[i] = i;
        }
        ezs_printf("start bubblesort with sorted values, arraysize%d \n", k);
        for (j = 0; j < 100; j++) {
            bubblesort(g_data, k);
        }
        /* unsorted random values*/
        for (i = 0; i < k; ++i)
        {
            g_data[i] = rand();
        }
        ezs_printf("start bubblesort with unsorted values, arraysize %d \n", k);
        for (j = 0; j < 100; j++) {
            bubblesort(g_data, k);
        }
        /* XXX more ??*/
    }
}

uint32_t checksum(uint32_t array[], size_t N) {
	size_t i, j;
	uint32_t res = 0;
	for (i = 0; i < N; ++i) {
		res ^= array[i];
		for (j = 0; j < N; ++j) {
			res *= (array[i] + array[j]);
		}
	}
	return res;
}

uint32_t checksum_job(void) {
    /* array values should be unimportant */
    cyg_uint32 i;
    cyg_uint32 j;
    cyg_uint32 time;
    for (i = 64; i < 550; i*=2) {
        ezs_printf("\nstart cecksum with arraysize %u\n\n", i);
        for ( j = 0; j < 100; j++) {
            ezs_watch_start(&time); 
	        checksum(g_data, i);
            ezs_watch_stop(&time);
            ezs_printf(" %u \n", (time*ezs_counter_resolution_ps()/1000000));
        }   
    }
}


// A little test thread.
void thread(cyg_addrword_t arg)
{
	cyg_resolution_t resolution = cyg_clock_get_resolution(cyg_real_time_clock());
	const cyg_uint64 delay_ms = 5;
	const cyg_uint64 delay_ns = delay_ms * 1000000;
	const cyg_tick_count_t delay = (delay_ns * resolution.divisor)/resolution.dividend; //ticks

	while (1)
	{
		cyg_thread_delay(delay);	// Wait 5ms

		// do things here...
        checksum_job();
		// heapsort_job();
        // bubblesort_test();
        while(1);
	}
}

#define SAMPLE_SIZE 32
uint16_t g_sampled_values[SAMPLE_SIZE];

extern uint16_t sample_adc(void);
extern void initialize_adc(void);

void sample_job(void)
{
	static bool initialized = false;
	static int position = 0;

	if (not initialized)
	{
		initialize_adc();
		initialized = true;
	}
	g_sampled_values[position] = sample_adc();
	++position;
}

void cyg_user_start(void)
{
	// Setup counter
	ezs_counter_init();
	ezs_gpio_init();

	// Create test thread
	cyg_thread_create(11, &thread, 0, "thread1", my_stack, STACKSIZE,
	                  &thread_handle, &threaddata);

	// and set thread ready to run
	cyg_thread_resume(thread_handle);
}
