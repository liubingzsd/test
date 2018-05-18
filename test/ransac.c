#include "ransac.h"
#include "file_wr.h"

void get_two_random_points(int *index1,int *index2, const int num)
{
	*index1 = (uint16_t)(num * rand() / RAND_MAX - 1.0);
	*index2 = (uint16_t)(num * rand() / RAND_MAX - 1.0);

	if (*index1 % 2)
	{
		*index1 += 1;
	}
	if (*index2 % 2)
	{
		*index2 += 1;
	}
	if (*index1 >= 0 && *index1 < num && *index2 >= 0 && *index2 < num && index1 != index2)
	{
	}
	else
	{
		*index1 = 0;
		*index2 = 2;
	}
}

void calc_line_model_general(float *point, line_model_general *model,int index1, int index2)
{
	model->a = point[index2 + 1] - point[index1 + 1];
	model->b = point[index1] - point[index2];
	model->c = point[index2] * point[index1 + 1] - point[index1] * point[index2 + 1];
	model->b /= model->a;
	model->c /= model->a;
	model->a = 1.0f;
}


float cal_distance_general(float *p, line_model_general * model)
{
	float dist;
	dist = model->a * p[0] + model->b * p[1] + model->c;
	if (dist < 0) {
		dist *= -1;
	}
	return dist;
}

void ransac_compute_model(float *point, int num, line_model_general *best_model, int max_iter_cnt, float threshold)
{
	int iter = 0;
	line_model_general tmp_model;
	int index1,index2;
	int i;
	float dist;
	int total = 0, pre_total = 0;
	while (iter < max_iter_cnt) {
		get_two_random_points(&index1, &index2, num);
		calc_line_model_general(point, &tmp_model, index1, index2);
		for (i = 0; i < num; i++) {
			dist = cal_distance_general(point + i * 2, &tmp_model);
			if (dist < threshold) {
				total++;
			}
		}
		if (total > pre_total) {
			pre_total = total;
			best_model->a = tmp_model.a;
			best_model->b = tmp_model.b;
			best_model->c = tmp_model.c;
			total = 0;
		}
		iter++;
	}
	printf("line_mode  %f %f %f \n", best_model->a, best_model->b, best_model->c);
}

void test_ransac()
{
	float data[400 * 2] = { 0 };
	int m = 400;
	int n = 2;
	int i;
	line_model_general best_model;
	int max_iter_cnt = 100;
	float threshold = 1.0f;
	FILE *fp = fopen("data.dat", "r");
	if (fp == NULL)
	{
		printf("NO file\n");
	}
	else
	{
		read_data_from_file_float(fp, (float *)data, m, n);
		for (i = 0; i < 100; i++)
		{
			printf("ransac compute model \n");
			ransac_compute_model((float *)data, 800, &best_model, max_iter_cnt, threshold);
		}
	}
}
