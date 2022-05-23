

/**
 * @brief takes in a range of number from -1 to 1 and maps the number a range of numbers
 * 
 * @param num value between -1 and 1
 * @param range 
 * @return float mapped value
 */
float map(float num, int range);

/**
 * @brief waits for an amount of microsecond (not precise)
 * 
 * @param micro_secs 
 */
void wait_for(int micro_secs);

/**
 * @brief Converts date from big endian to little endian and vice versa
 * 
 * @param data data in original endianess
 * @return unsigned int data in new endianess
 */
unsigned int swap_endian(unsigned int data);

/**
 * @brief allocates memory using the pspkernel (allows more free space than malloc)
 * 
 * @param size - size to allocate in bytes
 * @return void* - pointer
 */
void * psp_malloc(int size);
