#include <pspthreadman.h>

#include "utils.h"

/**
 * @brief takes in a range of number from -1 to 1 and maps the number a range of numbers
 * 
 * @param num value between -1 and 1
 * @param range 
 * @return float mapped value
 */
float map(float num, int range) {
	num += 1;
	num /= 2;

	return num * range;
}

void wait_for(int micro_secs){
	int start_time = sceKernelGetSystemTimeLow();

	int loop = micro_secs * 32;
	volatile int x;
	for (x = 0; x < loop; x++)
	{
		/*do nothing*/  
		if (sceKernelGetSystemTimeLow() > start_time + micro_secs) break;    
	}
}
