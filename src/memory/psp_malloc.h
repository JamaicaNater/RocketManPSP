#pragma once

#include <cstdlib>

#include "globals.h"

void init_malloc();

void * psp_malloc(uint32_t bytes, const char * debug_info = "");

void psp_free(void * ptr);