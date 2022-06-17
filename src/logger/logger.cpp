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

    /**
     * @brief If assetion fails log and exit program
     * 
     * @param condition 
     * @param format 
     * @param ... 
     */
    void assert(bool condition, const char * format, ...){
        //#ifdef PSP_LOGGING
        if (condition) return;
        pspDebugScreenInit();

        // if (fd<0) {
        //     pspDebugScreenInit();
        //     pspDebugScreenPrintf("Failed to open logger file at %s, please make"
        //         " sure the path exists, exiting in 10 seconds", logger_file);
        //     sceKernelDelayThread(10 * SECOND);
        //     sceKernelExitGame();
        // }

        va_list args;
        char data[256] = "CRITICAL:assert() evaluated false: ";
        
        va_start( args, format );
        vsprintf(data+42, format, args);
        va_end( args );

        pspDebugScreenPrintf("%s, %s:%d", data, __FILE__, __LINE__);
        
        
        sceKernelDelayThread(10 * SECOND);
        
        sceKernelExitGame();

        
        //#endif
    }

    int __assert_fail(const char * condition, const char * file, int lineno, const char * format, ...){
        //#ifdef PSP_LOGGING
        pspDebugScreenInit();

        va_list args;
        char data[256];
        
        va_start( args, format );
        vsprintf(data, format, args);
        va_end( args );

        pspDebugScreenPrintf("assert() evaluated false: %s, at %s:%d comments: %s",condition, file, lineno, data);
        
        sceKernelDelayThread(10 * SECOND);
        
        sceKernelExitGame();

        return 1;
        //#endif
    }
    
}

