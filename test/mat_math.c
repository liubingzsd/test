#include "mat_math.h"
void mat_printf(double *a, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%13.7e ", *(a + i * n + j));
		}
		printf("\n");
	}
}
void mat_identity(double *a,int m)
{
	int i, j;
	for (i = 0; i < m; i++) 
	{
		for (j = 0; j < m; j++)
		{
			if (i == j)
			{
				a[i * m + j] = 1.0f;
			}
			else
			{
				a[i * m + j] = 0.0f;
			}

		}
	}
}

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
void mm_sub(double *a, double *b, double *c, int m, int n)
{
	int i, j;
	for (i = 0; i < m ; i++)
	{
		for (j = 0; j < n; j++)
		{
			c[i*n + j] = a[i*n + j] - b[i*n + j];
		}
	}
}
void mm_add(double *a, double *b, double *c, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			c[i*n + j] = a[i*n + j] + b[i*n + j];
		}
	}
}
void vv_mat(double *a, double *b, double *c, int m)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < m; j++)
		{
			c[i*m + j] = a[i] * b[j];
		}
	}
}