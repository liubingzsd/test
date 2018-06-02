#ifndef _FEATURE_POINT_H_
#define _FEATURE_POINT_H_
#include <stdio.h>
#include <stdint.h>
#define MAX_FEATURE_POINT 200

typedef struct
{
	int16_t num;
	int16_t buf[MAX_FEATURE_POINT];
}feature_point_s;

void test_feature_point();
void write_feature_point_data_to_file(char *str, feature_point_s *point);

#endif