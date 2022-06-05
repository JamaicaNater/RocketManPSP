#pragma once

#include <stdio.h>

namespace PSP_LOGGER {
    enum levels{DEBUG, INFO, WARNING, ERROR, CRITICAL};

    /**
     * Takes in the log level and a formatted string and outputs it to logger.log
     * 
     * @param level - level
     * @param format - string w/ formatting
     * @param ... 
     */
    void log(unsigned char level, const char* format, ... );

    /**
     * @brief Closes the log file
     * 
     */
    void close_log();

    /**
     * @brief If condition evaluates to false close the program and output the 
     * given notes.
     * 
     * @param condition 
     * @param format - Notes
     * @param ... 
     */
    void assert(bool condition, const char * format, ...);
}
