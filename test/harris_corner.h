#ifndef _HARRIS_CORNER_H_
#define _HARRIS_CORNER_H_
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "feature_point.h"

typedef struct
{
	feature_point_s point;
	float response_max_val;
	float thres;
	float k;
}harris_corner_detector_t;
void test_harris_corner();
#endif

