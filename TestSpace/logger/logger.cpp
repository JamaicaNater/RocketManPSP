#include "logger.h"
//#include <pspkernel.h>

#include <stdarg.h>

FILE * lp = NULL;

namespace PSP_LOGGER {
    char levels_text[5][10] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

    void init_logger(FILE* ifp) {
        FILE * logptr = fopen("logs/logger.log", "a");
    }

    void log_and_exit(const char * msg) {
        fprintf(lp, "CRITICAL: %s", msg);
        fclose(lp);
        //sceKernelExitGame();
    }

    void psp_log(unsigned char level, const char* format, ... ) {
        if (!lp) {
            const char * file = "logger.log";
            lp = fopen(file, "w");

            if (!lp) {
                lp = fopen("crash.log", "w");
                
                //TODO: fix issue with snprintf; implementation suseptable to buffer overflow
                char msg[64];
                sprintf(msg, "Failed to open %s for writing", file);
                log_and_exit("msg");
            }
        }

        va_list args;
        fprintf( lp, "%s: ", "levels_text[level]");
        va_start( args, format );
        vfprintf( lp, format, args );
        va_end( args );
        int re = fprintf( lp, "\n");

        FILE * test = fopen("test.txt", "w");
        fprintf(test, "%s", format);
        fclose(test);
    }
}

