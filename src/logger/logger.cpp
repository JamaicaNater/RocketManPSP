#include "logger.h"
#include "../utils.h"
#include "../globals.h"
#include <pspkernel.h>
#include <stdio.h>
#include<cstring>

#include <stdarg.h>


#ifdef PSP_LOGGING
const char * logger_file = "umd0:/logs/logger.log";
SceUID fd = sceIoOpen(logger_file, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
#endif


char levels_text[6][10] = {"DEBUG-H", "DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

void log(unsigned char level, const char* format, ... ) {
    #ifdef PSP_LOGGING
    if (fd<0) {
        pspDebugScreenInit();
        pspDebugScreenPrintf("Failed to open logger file at %s, please make sure the path exists, exiting in 10 seconds", logger_file);
        sceKernelDelayThread(10 * SECOND);
        sceKernelExitGame();
    }

    va_list args;
    char data[128];
    sceIoWrite(fd, levels_text[level], strlen(levels_text[level])); // or strlen(data)
    sceIoWrite(fd, (void *)": ", 2); // or strlen(data)
    va_start( args, format );
    vsprintf(data, format, args);
    va_end( args );
    sceIoWrite(fd, data, strlen(data));
    
    sceIoWrite(fd, (void *)"\n", 1); // or strlen(data)

    if (level == CRITICAL) {
        close_log();
        sceKernelExitGame();
    }
    #endif
} 

void close_log() {
    #ifdef PSP_LOGGING
    if(fd >= 0) sceIoClose(fd);
    #endif
}

int __assert_fail(const char * condition, const char * file, int lineno, const char * format, ...){
    pspDebugScreenInit();

    va_list args;
    char va_data[256];
    char data[512];
    
    va_start( args, format );
    vsprintf(va_data, format, args);
    va_end( args );

    sprintf(data, "assert() evaluated false: %s, at %s:%d comments: %s", condition, file, lineno, va_data);

    pspDebugScreenPrintf("%s", data);

    sceKernelDelayThread(5 * SECOND);
    #ifdef PSP_LOGGING
    log(CRITICAL, data);
    #endif
    close_log();
    sceKernelExitGame();

    return 1;
}
    


