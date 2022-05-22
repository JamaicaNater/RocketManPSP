#include "logger.h"
#include "../utils.h"
#include <pspkernel.h>
#include <stdio.h>
#include<cstring>

#include <stdarg.h>


const char * logger_file = "umd0:/logs/logger.log";
SceUID fd = sceIoOpen(logger_file, PSP_O_WRONLY | PSP_O_CREAT, 0777);

namespace PSP_LOGGER {
    char levels_text[5][10] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

    void psp_log(unsigned char level, const char* format, ... ) {
        if (fd<0) {
            pspDebugScreenInit();
            pspDebugScreenPrintf("Failed to open logger file at %s, please make sure the path exists, exiting in 10 seconds", logger_file);
            wait_for(10*1000*1000);
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
    } 

    void close_log() {
        if(fd >= 0) sceIoClose(fd);
    }
}

