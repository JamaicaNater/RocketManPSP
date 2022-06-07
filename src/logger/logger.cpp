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

namespace PSP_LOGGER {
    char levels_text[5][10] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};
    
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

    /**
     * @brief If assetion fails log and exit program
     * 
     * @param condition 
     * @param format 
     * @param ... 
     */
    void assert(bool condition, const char * format, ...){
        #ifdef PSP_LOGGING
        //if (condition) return;

        if (fd<0) {
            pspDebugScreenInit();
            pspDebugScreenPrintf("Failed to open logger file at %s, please make sure the path exists, exiting in 10 seconds", logger_file);
            sceKernelDelayThread(10 * SECOND);
            sceKernelExitGame();
        }

        va_list args;
        char data[128];
        if (!condition) sceIoWrite(fd, (void*)"CRITICAL:assert() evaluated false: ", 42);
        else sceIoWrite(fd, (void*)"DEBUG: (assertion) ", 19);
        va_start( args, format );
        vsprintf(data, format, args);
        va_end( args );
        sceIoWrite(fd, data, strlen(data));
        
        sceIoWrite(fd, (void *)"\n", 1); // or strlen(data)

        if (!condition){
            close_log();
            sceKernelExitGame();
        }
        
        #endif
    }
    
}

