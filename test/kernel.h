#ifndef _KERNEL_H_
#define _KERNEL_H_
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum kernel_type {
	KERNEL_UINT8,
	KERNEL_INT8,
	KERNEL_UINT16,
	KERNEL_INT16,
	KERNEL_F32
};

typedef struct
{
	enum kernel_type type;
	uint8_t size;
	float scale;
	uint32_t buf_size;
	void *buf;
}kernel_t;

void kernel_create(kernel_t *kernel,uint8_t size, enum kernel_type type);
void gauss_kernel_2d(kernel_t *kernel, float sigma1, float sigma2);
void calc_gauss_ker_2d(float *ker, int size, float sigma1, float sigma2);
void kernel_free(kernel_t *kernel);
void test_filter_kernel();
#endif
