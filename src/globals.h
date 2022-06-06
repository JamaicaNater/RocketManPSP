#ifndef PSP_GLOBS
#define PSP_GLOBS

#define MAP_SIZE 3000
#define SCREEN_HEIGHT 272
#define SCREEN_WIDTH_RES 480
#define SCREEN_WIDTH 512
#define SECOND 1000000
#define MILLISECOND 1000
#define GAME_FRAME_TIME SECOND / 60

extern unsigned char * noise_map;
extern int camera_x;
extern unsigned int curr_time;
#endif