#include <stdio.h>
#include <malloc.h>
#include "file_wr.h"

void read_data_from_file_float(FILE *fp, float *data, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			fscanf_s(fp, "%f", data + i * n + j);
		}
	}
}
void read_data_from_file_uint8(FILE *fp,uint8_t *data, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			fscanf_s(fp, "%hhd", data + i * n + j);
		}
	}
}

void write_data_to_file_float(FILE *fp, float *data, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			fprintf(fp, "%f ", *(data + i * n + j));
		}
		fprintf(fp, "\n");
	}
}

void write_data_to_file_uint8(FILE *fp, uint8_t *data, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			fprintf(fp, "%d ", *(data + i * n + j));
		}
		fprintf(fp, "\n");
	}
}

void write_data_to_file_int16(FILE *fp, int16_t *data, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			fprintf(fp, "%d ", *(data + i * n + j));
		}
		fprintf(fp, "\n");
	}
}

void test_file_wr()
{
	uint8_t data[480 * 640] = { 0 };
	uint8_t data1[480 * 640] = { 0 };
	uint8_t data2[640] = { 0 };
	int m = 480;
	int n = 640;
	FILE *fp = fopen("data.dat", "r");
	if (fp == NULL)
	{
		printf("NO file fp\n");
	}
	else 
	{
		read_data_from_file_uint8(fp, data, m, n);
	}

	FILE *fp1 = fopen("data1.dat", "w");
	if (fp == NULL)
	{
		printf("NO file fp1\n");
	}
	else 
	{
		write_data_to_file_uint8(fp1, data, m, n);
	}
	fclose(fp1);
}


void test_file_wr_malloc()
{
	uint8_t *p;
	p = (uint8_t *)malloc(480 * 640);
	if (p) {
		printf("mem allocated at 0x%x\n", (unsigned int)p);
	}
	else {
		printf("mem allocated filed \n");
	}
	int m = 480;
	int n = 640;
	FILE *fp = fopen("data.dat", "r");
	if (fp == NULL)
	{
		printf("NO file fp\n");
	}
	else 
	{
		read_data_from_file_uint8(fp, p, m, n);
	}
	fclose(fp);
	FILE *fp1 = fopen("data1.dat", "w+");
	if (fp1 == NULL)
	{
		printf("NO file fp1\n");
	}
	else
	{
		write_data_to_file_uint8(fp1, p, m, n);
	}
	fclose(fp1);
	free(p);
}
void test_file_read_float()
{
	float data[400][2] = { 0 };
	int m = 400;
	int n = 2;
	FILE *fp = fopen("data.dat", "r");
	if (fp == NULL)
	{
		printf("NO file\n");
	}
	else
	{
		read_data_from_file_float(fp, (float *)data, m, n);
	}
}
