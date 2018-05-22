#ifndef _MATCH_H_
#define _MATCH_H_
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
	uint16_t s_x;
	uint16_t s_y;
	uint16_t best_x;
	uint16_t best_y;
	uint32_t sad_min_val;
	uint32_t sad_val;
}sad_t;

typedef struct
{
	uint16_t s_x;
	uint16_t s_y;
	uint16_t x_l;
	uint16_t y_l;
	uint32_t max_match_num;
	uint32_t thres;
	uint32_t match_num;
}ssad_t;

void test_sad();
#endif