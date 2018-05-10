#include "qr_solver.h"
#include "mat_math.h"
bool qr_cmp(double *a, int m, int n, double *q)
{
	int i, j, k, l, nn, p, jj;
	double u, alpha, w, t;
	if (m < n)
	{
		printf("qr_decompose failed \n");
		return false;
	}
	/* q set diag matrix */
	for (i = 0; i <= m - 1; i++)
	{
		for (j = 0; j <= m - 1; j++)
		{
			l = i * m + j;
			q[l] = 0.0f;
			if (i == j)
			{
				q[l] = 1.0f;
			}
		}
	}

	nn = n;
	if (m == n)
	{
		nn = m - 1;
	}
	for (k = 0; k <= nn - 1; k++)
	{
		u = 0.0;
		for (i = k; i <= m - 1; i++)
		{
			w = fabs(a[i * n + k]);
			if (w > u)
			{
				u = w;                    //u = max(a[j][k])
			}
		}
		alpha = 0.0;
		for (i = k; i <= m - 1; i++)
		{
			t = a[i * n + k] / u;
			alpha = alpha + t * t;        //alpha = sum (a[i][k]/u)*(a[i][k]/u)
		}
		l = k * n + k;
		if (a[l] > 0.0f)
		{
			u = -u;                      //-sign(a[k][k]) * u
		}
		alpha = u * sqrt(alpha);        //alpha = -sign(a[k][k]) * u * sqrt(alpha);
		if (fabs(alpha) + 1.0f == 1.0f)
		{
			printf("qr_decompose failed \n");
			return false;
		}
		u = sqrt(2.0 * alpha * (alpha - a[l]));
		if ((u + 1.0f) != 1.0f)
		{
			a[l] = (a[l] - alpha) / u;          //u[k] =  a[k][k] = (a[k][k] - alpha)/u;
			for (i = k + 1; i <= m - 1; i++)
			{
				p = i * n + k;
				a[p] = a[p] / u;               //u[i] = a[i][k] = a[i][k]/u;   
			}
			/* Qk * q */
			for (j = 0; j <= m - 1; j++)
			{
				t = 0.0;
				for (jj = k; jj <= m - 1; jj++)
				{
					t = t + a[jj * n + k] * q[jj * m + j]; //t = sum(a[jj] * q[][])
				}
				for (i = k; i <= m - 1; i++)
				{
					p = i * m + j;
					q[p] = q[p] - 2.0f * t * a[i * n + k];
				}
			}
			/* Qk * A */
			for (j = k + 1; j <= n - 1; j++)
			{
				t = 0.0;
				for (jj = k; jj <= m - 1; jj++)
				{
					t = t + a[jj*n + k] * a[jj*n + j];
				}
				for (i = k; i <= m - 1; i++)
				{
					p = i * n + j;
					a[p] = a[p] - 2.0*t*a[i*n + k];
				}
			}
			a[l] = alpha;
			for (i = k + 1; i <= m - 1; i++)
			{
				a[i * n + k] = 0.0f;                 //left_upper_matrix
			}
		}
	}
	for (i = 0; i <= m - 2; i++)                //mat trans
	{
		for (j = i + 1; j <= m - 1; j++)
		{
			p = i * m + j;
			l = j * m + i;
			t = q[p];
			q[p] = q[l];
			q[l] = t;
		}
	}
	return true;
}
bool qr_solver(double *a,int m,int n,double *b,double *q)
{
	int i, j;
	double *c, d;
	c = malloc(n * sizeof(double));
	if (qr_cmp(a, m, n, q))
	{
		for (i = 0; i < n; i++)
		{
			d = 0.0f;
			for (j = 0; j < m; j++)
			{
				d += q[j * m + i] * b[j];
			}
			c[i] = d;
		}
		b[n - 1] = c[n - 1] / a[n * n - 1];
		for (i = n - 2; i >= 0; i--)
		{
			d = 0.0f;
			for (j = i + 1; j < n; j++)
			{
				d += a[i * n + j] * b[j];
			}
			b[i] = (c[i] - d) / a[i * n + i];
		}
		free(c);
		return true;
	}
	else
	{
		free(c);
		return false;
	}
}


void test_qr_solver()
{
	int i, m, n;
	double a[4][3] = { 
		{ 1.0,1.0,-1.0 },
		{ 2.0,1.0,0.0 },
		{ 1.0,-1.0,0.0 },
		{ -1.0,2.0,1.0 } 
	};
	double b[4] = { 2.0,-3.0,1.0,4.0 };
	double q[4][4];
	m = 4; n = 3;
	if (qr_solver((double *)a, m, n, b, (double *)q))
	{
		for (i = 0; i < n; i++)
		{
			printf("x(%d) = %12.7f\n", i, b[i]);
		}
		printf("\n");
		printf("MAT Q IS:\n");
		mat_printf((double *)q, m, m);
		printf("\n");
		printf("MAT R IS:\n");
		mat_printf((double *)a,m,n);
	}
	else
	{
		printf("qr_solver failed  \n");
	}
}
void test_qr_cmp()
{
	double q[4][4];
	double a[4][3] = { 
		{ 1.0,1.0,-1.0 },
		{ 2.0,1.0,0.0 },
		{ 1.0,-1.0,0.0 },
		{ -1.0,2.0,1.0 } 
	};
	int m = 4, n = 3;
	if (qr_cmp((double *)a, m, n, (double *)q)) 
	{
		printf("MAT Q IS:\n");
		mat_printf((double *)a, m, m);
		printf("\n");
		printf("MAT R IS:\n");
		mat_printf((double *)a, m, n);
		printf("\n");
	}
	else
	{
		printf("qr_cmp failed \n");
	}
}

