#pragma once
#include <cstdlib>

void init_malloc();

void * psp_malloc(unsigned int bytes, const char * debug_info = "");

void psp_free(void * ptr);