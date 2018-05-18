#include "mat_math.h"

void mat_printf_float(float *mat, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%f ", *(mat + i * n + j));
		}
		printf("\n");
	}
}

void mat_printf(double *a, int m, int n)
{
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%12.7f ", *(a + i * n + j));
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
void mv_mul(double *a, double *b, double *c, int m, int n)
{
	int i, j, u;
	for (i = 0; i < m; i++)
	{
		c[i] = 0;
		for (j = 0; j < n; j++)
		{
			u = i * n + j;
			c[i] += a[u] * b[j];
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
void vv_mul_mat(double *a, double *b, double *c, int m)
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
void vector_create(double *a,int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		a[i] = 0.0f;
	}
}
void sv_mul(double *a,int length,double s)
{
	int i;
	for (i = 0; i < length; i++)
	{
		a[i] *= s;
	}
}
void vv_add(double *a, double *b, double *c, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		c[i] = a[i] + b[i];
	}
}


void vector_printf(double *a, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		printf("%12.7f ", a[i]);
	}
	printf("\n");

}
