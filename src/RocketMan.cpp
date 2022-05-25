#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspthreadman.h>
#include <cstring>
#include <cstdlib>
#include "gamestate.h"

#include "Projectile.hpp"
#include "utils.h"
#include "logger/logger.h"

#include "gfx.hpp"
#include "FastNoise/FastNoise.hpp"

#define MICROSECONDS 1000 * 1000
#define printf pspDebugScreenPrintf

//Boilerplate PSP code
PSP_MODULE_INFO("Tutorial", 0, 1, 0);
PSP_HEAP_SIZE_KB(2048);
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

/**
 * @brief Code written to interrupt the code for the home page
 * 
 * @param args size of argp in bytes
 * @param argp bytes 0-3: ID of the background thread. bytes 4-7: ID of the main thread.
 * @return int 
 */
int interrupt_titlescreen(SceSize args, void* argp){
	unsigned int * arg_arr = (unsigned int*)argp;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceCtrlData ctrlData;
	bool in_titlescreen = true;

	while (1)
	{
		sceCtrlReadBufferPositive(&ctrlData, 1);
		if (ctrlData.Buttons & PSP_CTRL_START && in_titlescreen){
			sceKernelTerminateThread(arg_arr[0]);
			sceKernelDeleteThread(arg_arr[0]);
			sceKernelWakeupThread(arg_arr[1]);
			PSP_LOGGER::psp_log(PSP_LOGGER::INFO, "Waking thread: %u", arg_arr[1]);
			in_titlescreen = false;
			sceKernelExitDeleteThread(PSP_THREAD_STOPPED);
		}
		sceKernelDelayThread(100);
	}
	return 0;
}

const int MAP_SIZE = 1000;
unsigned char noise_map[MAP_SIZE];
int FRAMETIME = MICROSECONDS / 60;

int main()
{  	
	setupCallbacks();
	pspDebugScreenInit();

	GFX::load_terrain_textures();
	GFX::init();

	unsigned int start_time = 0, end_time = 0;

	FastNoiseLite noise(sceKernelGetSystemTimeLow());
	noise.SetFrequency(.008f);
	//noise.SetDomainWarpAmp(30.0f);
	for(int i = 0; i < MAP_SIZE; i++) {
		noise_map[i] = (char)map(noise.GetNoise((float)i*.8f, 0.0f), 100) + 100; // MIN hieght = 40 max hieght = 150 + 40
	}

	//memset(noise_map+600, 100, 400);

	unsigned int home_thid = sceKernelCreateThread("homescreen_thread", GFX::do_homescreen, 0x12, 0xaFA0, 0, NULL);
	if (home_thid >= 0) sceKernelStartThread(home_thid, 0, NULL);
	else PSP_LOGGER::psp_log(PSP_LOGGER::ERROR, "failed to create thread");

	int main_thid = sceKernelGetThreadId();
	if (main_thid<0) PSP_LOGGER::psp_log(PSP_LOGGER::CRITICAL, "failed to obtain main thread ID");

	unsigned int args[2] = {home_thid, (unsigned int)main_thid}; // If main_thid is not positive the program terminates
	int inter_home_thid = sceKernelCreateThread("interrupt_homescreen_thread", interrupt_titlescreen, 0x12, 0xaFA0, 0, NULL);
	if (inter_home_thid >= 0) sceKernelStartThread(inter_home_thid, 8, args);
	else PSP_LOGGER::psp_log(PSP_LOGGER::ERROR, "failed to create thread");
	sceKernelSleepThread();


	float physics_time_delta = 0.0f;
	GameState state = GameState();
	state.init(noise_map, MAP_SIZE);
	while (1)
	{
		start_time = sceKernelGetSystemTimeLow(); // For FPS calculation
		
		pspDebugScreenSetXY(0,0);

		state.update(start_time);
		state.draw();

		end_time = sceKernelGetSystemTimeLow();
		physics_time_delta = (end_time - start_time) / static_cast<float>(1000*1000);
		printf("fps: %.1f", 1 / (physics_time_delta) );
		//printf("fps: %.1f, cam_x %d, x: %d, y: %d, angle: %d, cam_lock_l: %d\n cam_lock_r: %d, jump height %d", 1 / (physics_time_delta ), cam_pos_x, player.vector.x, player.vector.y, player.vector.get_angle(), cam_locked_left, cam_locked_right, player.jump_height);

		sceDisplayWaitVblankStart();
	}
}
