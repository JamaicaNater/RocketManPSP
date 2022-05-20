#include <stdio.h>
unsigned int swap_endian(unsigned int data)
{
    return  (data&0x0000FF00) | (data&0xFF000000) | (data&0x00FF0000)>>16 | (data&0x000000FF)<<16;
}

int main() {
    int data = swap_endian(0xFF607080);

    printf("%0x ", data);
}