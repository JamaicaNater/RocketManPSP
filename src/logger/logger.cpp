#include "logger.h"
#include <pspkernel.h>
#include <stdio.h>
#include<cstring>

#include <stdarg.h>


FILE * lp = NULL;
SceUID fd = sceIoOpen("umd0:/logs/logger.log", PSP_O_WRONLY | PSP_O_CREAT, 0777);

namespace PSP_LOGGER {
    char levels_text[5][10] = {"DEBUG\0", "INFO", "WARNING", "ERROR", "CRITICAL"};

    void init_logger(FILE* ifp) {
        FILE * logptr = fopen("logs/logger.log", "a");
    }

    void log_and_exit(const char * msg) {
        fprintf(lp, "CRITICAL: %s", msg);
        fclose(lp);
        sceKernelExitGame();
    }

    void psp_log(unsigned char level, const char* format, ... ) {
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
            fclose(lp);
            sceKernelExitGame();
        }
    } 

    void close_log() {
        if(fd >= 0) sceIoClose(fd);
    }
}

