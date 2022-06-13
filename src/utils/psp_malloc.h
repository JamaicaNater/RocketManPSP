#pragma once
#include <cstdlib>

void init_malloc();

void * psp_malloc(size_t bytes, const char * debug_info = "");

void psp_free(void * ptr);