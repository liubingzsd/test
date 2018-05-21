#include "mat_math.h"
#include "kernel.h"
#define PI 3.1415926
void calc_gauss_ker_2d(float *ker, int size, float sigma1, float sigma2)
{
	int x_cen, y_cen;
	int i, j;
	float sig,xtmp,ytmp;
	float sum = 0, sum1 = 0,tmp;
	x_cen = y_cen = (size - 1) / 2;
	sig = 2 * sigma1 * sigma2;
	for (i = 0; i < size; i++)
	{
		xtmp = (i - x_cen) * (i - x_cen);
		for (j = 0; j < size; j++)
		{
			ytmp = (j - y_cen) * (j - y_cen);
			tmp = *(ker + i * size + j) = exp(-(xtmp + ytmp) / sig) / (sig);
			sum += tmp;
		}
	}
	/*normalized to 1*/
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			*(ker + i * size + j) /= sum;
		}
	}
}



void gauss_kernel_2d(kernel_t *kernel,float sigma1, float sigma2)
{
	int x_cen, y_cen;
	int i, j;
	float sig, xtmp, ytmp;
	float sum = 0, sum1 = 0, tmp;
	float *kernel_buf = (float *)kernel->buf;
	x_cen = y_cen = (kernel->size - 1) / 2;
	sig = 2 * sigma1 * sigma2;
	for (i = 0; i < kernel->size; i++)
	{
		xtmp = (i - x_cen) * (i - x_cen);
		for (j = 0; j < kernel->size; j++)
		{
			ytmp = (j - y_cen) * (j - y_cen);
			tmp = *(kernel_buf + i * kernel->size + j) = exp(-(xtmp + ytmp) / sig) / (sig);
			sum += tmp;
		}
	}
	/*normalized to 1*/
	for (i = 0; i < kernel->size; i++)
	{
		for (j = 0; j < kernel->size; j++)
		{
			*(kernel_buf + i * kernel->size + j) /= sum;
		}
	}
}

void kernel_create(kernel_t *kernel,uint8_t size, enum kernel_type type)
{
	kernel->size = size;
	kernel->scale = 1;
	if (type == KERNEL_UINT8 || type == KERNEL_INT8)
	{
		kernel->buf_size = sizeof(uint8_t) * size * size;
	}
	else if (type == KERNEL_UINT16 || type == KERNEL_INT16) 
	{
		kernel->buf_size = sizeof(uint16_t) * size * size;
	}
	else if(type == KERNEL_F32) 
	{
		kernel->buf_size = sizeof(float) * size * size;
	}
	kernel->buf = malloc(kernel->buf_size);
	if (kernel->buf)
	{
//		printf("kernel buf allocated at 0x%x \n", (unsigned int)kernel->buf);
	}
	else
	{
		printf("___func___ kernel_create \n");
		printf("kernel buf allocated failed \n");
	}
}

void kernel_free(kernel_t *kernel)
{
	if (kernel->buf != NULL)
	{
		free(kernel->buf);
	}
	kernel->buf = NULL;
}

void test_filter_kernel()
{
	#define  M  7
	kernel_t kernel;
	float sigma1 = 2;
	float sigma2 = 2;
	float gauss[M * M] = { 0 };
	calc_gauss_ker_2d(gauss, M, sigma1, sigma2);
	mat_printf_float(gauss,M,M);
	kernel_create(&kernel,M,KERNEL_F32);
	gauss_kernel_2d(&kernel, sigma1, sigma2);
	mat_printf_float((float *)kernel.buf, M, M);
	kernel_free(&kernel);
	
}