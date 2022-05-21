#include "utils.h"

/**
 * @brief takes in a range of number from -1 to 1 and maps the number a range of numbers
 * 
 * @param num value between -1 and 1
 * @param range 
 * @return float mapped value
 */
float map(float num, int range) {
	num += 1;
	num /= 2;

	return num * range;
}
