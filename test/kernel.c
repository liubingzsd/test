#include "mat_math.h"
#include "kernel.h"
#define PI 3.1415926
void gauss_ker_2d(float *ker,int size,float sigma1,float sigma2)
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


void test_filter_kernel()
{
	#define  M  7
	float sigma1 = 2;
	float sigma2 = 2;
	float gauss[M * M] = { 0 };
	gauss_ker_2d(gauss, M, sigma1, sigma2);
	mat_printf_float(gauss,M,M);
}