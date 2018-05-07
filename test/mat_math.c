#include "mat_math.h"
void mm_mul(double *a, double *b, double *c ,int m, int n, int k)
{ 
	int i, j, l, u;
	for (i = 0; i <= m - 1; i++)
	{
		for (j = 0; j <= k - 1; j++)
		{
			u = i * k + j;
			c[u] = 0.0f;
			for (l = 0; l <= n - 1; l++)
			{
				c[u] += a[i*n + l] * b[l*k + j];
			}
				
		}
	}

}