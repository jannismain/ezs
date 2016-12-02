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

#define HEAPSORT_DATA_SIZE 500
static uint32_t heapsort_data[] = {251, 375, 492, 311, 376, 439, 493, 279, 312, 343, 377, 407, 440, 471, 494, 263, 280, 295, 313, 327, 344, 359, 378, 391, 408, 423, 441, 455, 472, 487, 495, 255, 264, 271, 281, 287, 296, 303, 314, 319, 328, 335, 345, 351, 360, 367, 379, 383, 392, 399, 409, 415, 424, 431, 442, 447, 456, 463, 473, 479, 488, 496, 248, 131, 256, 259, 265, 139, 272, 275, 282, 147, 288, 291, 297, 155, 304, 307, 315, 163, 320, 323, 329, 171, 336, 339, 346, 179, 352, 355, 361, 187, 368, 371, 380, 195, 384, 387, 393, 203, 400, 403, 410, 211, 416, 419, 425, 219, 432, 435, 443, 227, 448, 451, 457, 235, 464, 467, 474, 243, 480, 483, 489, 497, 244, 122, 249, 63, 140, 77, 257, 127, 260, 129, 266, 67, 268, 133, 273, 135, 276, 137, 283, 71, 156, 141, 289, 143, 292, 145, 298, 75, 300, 149, 305, 151, 308, 153, 316, 79, 172, 93, 321, 159, 324, 161, 330, 83, 332, 165, 337, 167, 340, 169, 347, 87, 188, 173, 353, 175, 356, 177, 362, 91, 364, 181, 369, 183, 372, 185, 381, 95, 204, 109, 385, 191, 388, 193, 394, 99, 396, 197, 401, 199, 404, 201, 411, 103, 220, 205, 417, 207, 420, 209, 426, 107, 428, 213, 433, 215, 436, 217, 444, 111, 236, 125, 449, 223, 452, 225, 458, 115, 460, 229, 465, 231, 468, 233, 475, 119, 252, 237, 481, 239, 484, 241, 490, 123, 498, 0, 245, 58, 247, 121, 250, 1, 94, 32, 157, 17, 190, 68, 258, 10, 130, 46, 261, 37, 262, 128, 267, 7, 134, 64, 269, 31, 270, 132, 274, 22, 138, 66, 277, 65, 278, 136, 284, 4, 142, 36, 285, 33, 286, 76, 290, 16, 146, 70, 293, 69, 294, 144, 299, 15, 150, 72, 301, 35, 302, 148, 306, 34, 154, 74, 309, 73, 310, 152, 317, 3, 158, 40, 189, 21, 318, 84, 322, 18, 162, 78, 325, 45, 326, 160, 331, 9, 166, 80, 333, 39, 334, 164, 338, 38, 170, 82, 341, 81, 342, 168, 348, 8, 174, 44, 349, 41, 350, 92, 354, 20, 178, 86, 357, 85, 358, 176, 363, 19, 182, 88, 365, 43, 366, 180, 370, 42, 186, 90, 373, 89, 374, 184, 382, 2, 126, 48, 221, 25, 254, 100, 386, 14, 194, 62, 389, 53, 390, 192, 395, 11, 198, 96, 397, 47, 398, 196, 402, 30, 202, 98, 405, 97, 406, 200, 412, 6, 206, 52, 413, 49, 414, 108, 418, 24, 210, 102, 421, 101, 422, 208, 427, 23, 214, 104, 429, 51, 430, 212, 434, 50, 218, 106, 437, 105, 438, 216, 445, 5, 222, 56, 253, 29, 446, 116, 450, 26, 226, 110, 453, 61, 454, 224, 459, 13, 230, 112, 461, 55, 462, 228, 466, 54, 234, 114, 469, 113, 470, 232, 476, 12, 238, 60, 477, 57, 478, 124, 482, 28, 242, 118, 485, 117, 486, 240, 491, 27, 246, 120, 499, 59};


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

void heapsort_job(void) {
	heapsort(g_data, DATA_SIZE);
}

void bubblesort(uint32_t array[], unsigned int N) {
	int i, j;
	for (i = 0; i < N - 1; ++i) {
		for (j = 0; j < N - i - 1; ++j) {
			if (array[j] > array[j + 1]) {
				// Bubble up!
				swap(array, i, j);
			}
		}
	}
}

void bubblesort_job(void) {
    bubblesort(g_data, DATA_SIZE);
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
            cyg_interrupt_disable();
            bubblesort(g_data, k);
            cyg_interrupt_enable();
        }
        /* unsorted random values*/
        for (i = 0; i < k; ++i)
        {
            g_data[i] = rand();
        }
        ezs_printf("start bubblesort with unsorted values, arraysize %d \n", k);
        for (j = 0; j < 100; j++) {
            cyg_interrupt_disable();
            bubblesort(g_data, k);
            cyg_interrupt_enable();
        }
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
    return checksum(g_data, DATA_SIZE);
}

void checksum_test(void) {
    /* array values should be unimportant */
    cyg_uint32 i;
    cyg_uint32 j;
    cyg_uint32 time;
    
    for (i = 64; i < 550; i*=2) {
        ezs_printf("\nstart cecksum with arraysize %u\n\n", i);
        for ( j = 0; j < 100; j++) {
            cyg_interrupt_disable();
            ezs_watch_start(&time); 
	        checksum(g_data, i);
            ezs_watch_stop(&time);
            cyg_interrupt_enable();
            ezs_printf(" %u \n", (time*ezs_counter_resolution_ps()/1000000));
        }   
    }
}

void bubblesort_oszi(void){
    ezs_gpio_set(1);
    bubblesort(g_data, 512);
    ezs_gpio_set(0);
    ezs_delay_us(10 * 1000);
}

void test_heapsort(void) {
    cyg_uint32 time;
    cyg_uint32 result;
    cyg_uint32 i;

    cyg_interrupt_disable();
    ezs_watch_start(&time);
    heapsort(heapsort_data, HEAPSORT_DATA_SIZE);
    result = ezs_watch_stop(&time);
    cyg_interrupt_enable();

    result -= 22;
    ezs_printf("heapsort special data: %u us\n", result*ezs_counter_resolution_ps()/1000000);

    for (i = 0; i < HEAPSORT_DATA_SIZE; i++) {
        heapsort_data[i] = 1;
    }
    cyg_interrupt_disable();
    ezs_watch_start(&time);
    heapsort(heapsort_data, HEAPSORT_DATA_SIZE);
    result = ezs_watch_stop(&time);
    cyg_interrupt_enable();

    result -= 22;
    ezs_printf("heapsort constant: %u us\n", result*ezs_counter_resolution_ps()/1000000);


    for (i = 0; i < HEAPSORT_DATA_SIZE; i++) {
        heapsort_data[i] = i;
    }
    cyg_interrupt_disable();
    ezs_watch_start(&time);
    heapsort(heapsort_data, HEAPSORT_DATA_SIZE);
    result = ezs_watch_stop(&time);
    cyg_interrupt_enable();

    result -= 22;
    ezs_printf("heapsort sorted: %u us\n", result*ezs_counter_resolution_ps()/1000000);


    for (i = 0; i < HEAPSORT_DATA_SIZE; i++) {
        heapsort_data[i] = HEAPSORT_DATA_SIZE - i;
    }
    cyg_interrupt_disable();
    ezs_watch_start(&time);
    heapsort(heapsort_data, HEAPSORT_DATA_SIZE);
    result = ezs_watch_stop(&time);
    cyg_interrupt_enable();

    result -= 22;
    ezs_printf("heapsort sorted reverse: %u us\n", result*ezs_counter_resolution_ps()/1000000);
}

// A little test thread.
void thread(cyg_addrword_t arg) {
	cyg_resolution_t resolution = cyg_clock_get_resolution(cyg_real_time_clock());
	const cyg_uint64 delay_ms = 5;
	const cyg_uint64 delay_ns = delay_ms * 1000000;
	const cyg_tick_count_t delay = (delay_ns * resolution.divisor)/resolution.dividend; //ticks

    test_heapsort();

	while (1) {
//		cyg_thread_delay(delay);	// Wait 5ms

		// do things here...
        // cecksum_test();
        // bubblesort_test();
		// heapsort_job();
        // bubblesort_test();
//        bubblesort_oszi();
	}
}

#define SAMPLE_SIZE 32
uint16_t g_sampled_values[SAMPLE_SIZE];

extern uint16_t sample_adc(void);
extern void initialize_adc(void);

void sample_job(void) {
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

void cyg_user_start(void) {
	cyg_uint32 i;

    // Setup counter
	ezs_counter_init();
	ezs_gpio_init();

    /* data for sort */
    for (i = 0; i < DATA_SIZE; i++) 
    {
        g_data[i] = 1;
    }

    // Create test thread
	cyg_thread_create(11, &thread, 0, "thread1", my_stack, STACKSIZE,
	                  &thread_handle, &threaddata);

	// and set thread ready to run
	cyg_thread_resume(thread_handle);
}
