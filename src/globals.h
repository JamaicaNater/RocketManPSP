#ifndef PSP_GLOBS
#define PSP_GLOBS

#define MAP_SIZE 3000

#define SCREEN_HEIGHT 272
#define SCREEN_WIDTH_RES 480
#define SCREEN_WIDTH 512

#define PI 3.14159265359f

#define SECOND 1000000
#define MILLISECOND 1000

#define GAME_FRAME_TIME SECOND / 60

extern unsigned int curr_time;
extern unsigned int pause_time;

// Game Specific
extern unsigned char * noise_map;
extern int camera_x;
#endif