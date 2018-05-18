#ifndef _RANSAC_H_
#define _RANSAC_H_
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct
{
	float a;
	float b;
	float c;
}line_model_general;

typedef struct
{
	int p1;
	int p2;
}index_t;

void ransac_compute_model(float *point, int num, line_model_general *best_model, int max_iter_cnt, float threshold);
void test_ransac();
#endif
