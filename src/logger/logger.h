#include <stdio.h>

namespace PSP_LOGGER {
    enum levels{DEBUG, INFO, WARNING, ERROR, CRITICAL};
    /**
     * Takes in the log level and a formated string and outputs it to logger.log
     * 
     * @param level - level
     * @param format - string w/ formatting
     * @param ... 
     */
    
    void psp_log(unsigned char level, const char* format, ... );
    /**
     * @brief Closes the log file
     * 
     */
    void close_log();

    void assert_or_log(bool condition, const char * format, ...);
}
