#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspthreadman.h>
#include <cstdlib>

#include "globals.h"
#include "gamestate.h"
#include "utils.h"
#include "logger/logger.h"

#include "gfx.hpp"
#include "FastNoise/FastNoise.hpp"

unsigned char * noise_map = (unsigned char*)malloc(MAP_SIZE);

#define printf pspDebugScreenPrintf

//Boilerplate PSP code
PSP_MODULE_INFO("Tutorial", 0, 1, 0);
PSP_HEAP_SIZE_KB(4096);
PSP_MAIN_THREAD_STACK_SIZE_KB(2048);

int exit_calback(int arg1, int arg2, void* common)
{
	sceKernelExitGame();
	return 0;
}

int callbackThread(SceSize args, void* argp)
{
	int cbid = sceKernelCreateCallback("Exit Callback", exit_calback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();
	return 0;
}

void setupCallbacks()
{
	int thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, NULL);

	if (thid >= 0)
		sceKernelStartThread(thid, 0, NULL);

}
// End of boilerplate PSP code

int main()
{  	
	setupCallbacks();
	pspDebugScreenInit();

	GFX::load_terrain_textures();
	GFX::init();

	// Terrain Generation
	FastNoiseLite noise(sceKernelGetSystemTimeLow());
	noise.SetFrequency(.008f);
	for(int i = 0; i < MAP_SIZE; i++) {
		noise_map[i] = (char)map(noise.GetNoise((float)i*.8f, 0.0f), 100) + 100; // MIN hieght = 40 max hieght = 150 + 40
	}

	// FPS calculation
	float physics_time_delta = 0.0f;
	unsigned int start_time = 0, end_time = 0;

	GameState state = GameState();
	state.init();
	state.title_screen();
	while (1)
	{
		start_time = sceKernelGetSystemTimeLow(); // For FPS calculation
		
		pspDebugScreenSetXY(0,0);

		state.update(start_time);
		state.draw();

		end_time = sceKernelGetSystemTimeLow();
		physics_time_delta = (end_time - start_time) / static_cast<float>(1000*1000);
		printf("fps: %.1f", 1 / (physics_time_delta) );
		sceDisplayWaitVblankStart();
	}
}
