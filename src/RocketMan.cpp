#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspthreadman.h>
#include <cstdlib>

#include "globals.h"
#include "gamestate.h"
#include "utils.h"
#include "logger/logger.h"
#include "assert.h"
#include "utils/psp_malloc.h"

#include "graphics/gfx.hpp"
#include "../Dependencies/FastNoise/FastNoise.hpp"

unsigned char * noise_map = (unsigned char*)malloc(MAP_SIZE);
unsigned int curr_time = 0;
unsigned int pause_time = 0;
int camera_x = 0;

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
	int thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 
		0xFA0, 0, NULL);

	if (thid >= 0)
		sceKernelStartThread(thid, 0, NULL);

}
// End of boilerplate PSP code

int main()
{  	
	init_malloc();
	setupCallbacks();
	pspDebugScreenInit();

	GFX::load_terrain_textures();
	GFX::init();

	// Terrain Generation
	FastNoiseLite noise(sceKernelGetSystemTimeLow());
	noise.SetFrequency(.008f);
	for(int i = 0; i < MAP_SIZE; i++) {
		noise_map[i] = (char)map(noise.GetNoise((float)i*.8f, 0.0f), 100) + 100; 
	}

	// FPS calculation
	unsigned int start_time = 0, end_time = 0;

	GameState game_state = GameState();
	game_state.init();
	game_state.title_screen();

	curr_time = sceKernelGetSystemTimeLow();
	while (game_state.status_info.status != GameState::TERMINATED)
	{
		start_time = sceKernelGetSystemTimeLow(); // For FPS calculation
		pspDebugScreenSetXY(0,0);

		game_state.update();
		game_state.draw();

		end_time = sceKernelGetSystemTimeLow();
		printf("FPS: %.1f", static_cast<float>(SECOND) / (end_time - start_time));

		sceDisplayWaitVblankStart();
	}
}
