#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspthreadman.h>
#include <cstdlib>

#include "game/game_globals.h"
#include "game/gamestate.h"
#include "game/game_utils.h"

#include "globals.h"
#include "logger.h"
#include "psp_malloc.h"
#include "gfx.hpp"
#include "game/graphics/game_graphics.h"
#include "GUI/text_builder.h"

#include "../Dependencies/FastNoise/FastNoise.hpp"

unsigned char * noise_map = (unsigned char*)malloc(MAP_SIZE);
uint32_t curr_time = 0;
uint32_t pause_time = 0;
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
	pspDebugScreenInit();
	init_malloc();
	init_text_builder();
	setupCallbacks();
	

	load_terrain_textures();
	GFX::init();

	// Terrain Generation
	FastNoiseLite noise(sceKernelGetSystemTimeLow());
	noise.SetFrequency(.008f);
	for(int i = 0; i < MAP_SIZE; i++) {
		noise_map[i] = (char)map(noise.GetNoise((float)i*.8f, 0.0f), 100) + 100;
	}

	// FPS calculation
	uint32_t start_time = 0, end_time = 0;

	GameState game_state = GameState();
	game_state.init();
	game_state.title_screen();
	game_state.menu_screen();

	curr_time = sceKernelGetSystemTimeLow();
	while (game_state.status_info.status != GameState::TERMINATED)
	{
		start_time = sceKernelGetSystemTimeLow(); // For FPS calculation
		pspDebugScreenSetXY(0,0);

		game_state.update();
		game_state.draw();

		end_time = sceKernelGetSystemTimeLow();
		printf("FPS: %.1f", static_cast<double>(SECOND) / (end_time - start_time));

		sceDisplayWaitVblankStart();
	}
}
