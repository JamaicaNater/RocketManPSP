#include <stdio.h>

namespace PSP_LOGGER {
    enum levels{DEBUG, INFO, WARNING, ERROR, CRITICAL};

    void log_and_exit(const char * msg);
    void psp_log(unsigned char level, const char* format, ... );
    void init_logger(FILE* ifp);
    void close_log();
}
