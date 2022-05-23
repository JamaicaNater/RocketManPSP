#include <pspthreadman.h>

#include "utils.h"
#include "logger/logger.h"
#include "pspsysmem_kernel.h"

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

unsigned int swap_endian(unsigned int data)
{
	return  (data&0x0000FF00) | (data&0xFF000000) | (data&0x00FF0000)>>16 | (data&0x000000FF)<<16;
}

void * psp_malloc(int size) {
	SceUID uid = sceKernelCreateHeap(2, size+64, 1,"");

	if (uid>0)PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "uid: %d", uid);
	else {
		PSP_LOGGER::psp_log(PSP_LOGGER::ERROR, "uid: %d", uid);
		return NULL;
	}

    void * buf = (unsigned int *)sceKernelAllocHeapMemory(uid, size);
    if ((unsigned int)buf != 0xffffffff) PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Allocated %d bytes at 0x%0x",size, buf);
	else {
		PSP_LOGGER::psp_log(PSP_LOGGER::WARNING, "sceKernelAllocHeapMemory() returned 0x%0x", buf);
		return NULL;
	}

	return buf;
}
