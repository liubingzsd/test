#include "file_wr.h"
#include "feature_point.h"

void write_feature_point_data_to_file(char *str,feature_point_t *point)
{
	FILE *fp = fopen(str, "w+");
	if (fp == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		write_data_to_file_int16(fp, point->buf, point->num, 2);
	}
	fclose(fp);
}

void test_feature_point()
{
	char str[] = "feature_point.dat";
	feature_point_t point;
	write_feature_point_data_to_file(str,&point);
}