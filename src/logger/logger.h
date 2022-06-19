#pragma once

#include <stdio.h>

#define assert(cond, args...) (cond) ? 0 : __assert_fail(#cond, __FILE__, __FUNCTION__, __LINE__, args)

enum levels{DEBUG_H, DEBUG, INFO, WARNING, ERROR, CRITICAL};

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
 * @brief If an assertion fails run this code
 * 
 * @param condition - checked condition
 * @param file - file failed in
 * @param func - function failed in
 * @param lineno - line number failed in
 * @param format - text
 * @param ... 
 * @return int for ternary macro
 */
int __assert_fail(const char * condition, const char * file, const char * func, int lineno, const char * format, ...);

