#include "psp_malloc.h"

#include <cstdlib>

#include "../logger/logger.h"
struct MemNode
{
    void * ptr;
    size_t bytes;
};

struct MemMap {
    static const int MAX_SIZE = 256;
    MemNode arr[MAX_SIZE] = {0,0};
    unsigned char num_allocated = 0;
    size_t bytes_allocated = 0;

    int insert(MemNode m){
        int index = MAX_SIZE;

        while (index > -1)
        {
            if (!arr[index].ptr) {
                arr[index] = m;
                break;
            }
            index--;
        }

        bytes_allocated += m.bytes;
        num_allocated++;
        return index;
    }

    int remove(void * ptr){
        int index = MAX_SIZE;

        while (index > -1)
        {
            if (arr[index].ptr == ptr) {
                arr[index] = {0,0};
                break;
            }
            index--;
        }
        
        if (index > -1){ 
            bytes_allocated -= arr[index].bytes;
            PSP_LOGGER::log(PSP_LOGGER::DEBUG_H, "freed %llu bytes", arr[index].bytes);
        }
        num_allocated--;
        return index;
    }
};

MemMap map;

void init_malloc(){
    map.bytes_allocated = 0;
    map.num_allocated = 0;
    PSP_LOGGER::log(PSP_LOGGER::DEBUG_H, "Init: %u Objects in memory allocated to %lu bytes",map.num_allocated, map.bytes_allocated);
}

void * psp_malloc(size_t bytes, const char * debug_info){
    void * ptr = malloc(bytes);
    PSP_LOGGER::assert(map.insert({ptr, bytes})>-1, "psp_malloc inserted %u" 
        "bytes successfully", bytes);
    
    PSP_LOGGER::log(PSP_LOGGER::DEBUG_H, "Allocated %lu at %0x. Info %s", bytes, ptr, debug_info);
    PSP_LOGGER::log(PSP_LOGGER::DEBUG_H, "%u Objects in memory allocated to %lu bytes",map.num_allocated, map.bytes_allocated);

    return ptr;
}

void psp_free(void * ptr){
    PSP_LOGGER::assert(map.remove(ptr)>-1, "psp_free call on %0x" 
        "bytes successfully", ptr);
    
    PSP_LOGGER::log(PSP_LOGGER::DEBUG_H, "Attempting to free %0x", ptr);

    free(ptr);
    PSP_LOGGER::log(PSP_LOGGER::DEBUG_H, "Successfully freed %0x." 
        "%d Objects in memory allocated to %d bytes", 
        ptr, map.num_allocated, map.bytes_allocated);
}