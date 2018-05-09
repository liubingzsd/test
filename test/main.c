#include <stdio.h>
#include "gauss_jordan_solver.h"
#include "cholesky_solver.h"
#include "sv_decomp.h"
#include "mat_math.h"

void test_gauss_jordan_solver()
{
	int i;
	double a[4][4] = {
		{ 0.2368,0.2471,0.2568,1.2671 },
	{ 0.1968,0.2071,1.2168,0.2271 },
	{ 0.1581,1.1675,0.1768,0.1871 },
	{ 1.1161,0.1254,0.1397,0.1490 }
	};
	double b[4] = { 1.8471,1.7471,1.6471,1.5471 };
	if (gauss_jordan_solver(&a[0][0], &b[0], 4) != 0) {
		for (i = 0; i <= 3; i++)
			printf("x(%d)=%e\n", i, b[i]);
	}
}
void test_cholesky_solver()
{
	int i;
	 double a[4][4] = {
		{ 5.0,7.0,6.0,5.0 },
		{ 7.0,10.0,8.0,7.0 },
		{ 6.0,8.0,10.0,9.0 },
		{ 5.0,7.0,9.0,10.0 } 
	 };
	double d[4][2] = { 
		{ 23.0,92.0 },
		{ 32.0,128.0 },
		{ 33.0,132.0 },
		{ 31.0,124.0 } 
	};
	if (cholesky_solver(&a[0][0], 4, 2, &d[0][0]))
		for (i = 0; i <= 3; i++)
			printf("x(%d)=%13.7e,   %13.7e\n", i, d[i][0], d[i][1]);
}
void test_sv_decomp()
{
	double a[4][3] = {
		{ 1.0,1.0,-1.0 },
		{ 2.0,1.0,0.0 },
		{ 1.0,-1.0,0.0 },
		{ -1.0,2.0,1.0 }
	};
	double b[3][4] = {
		{ 1.0,1.0,-1.0,-1.0 },
		{ 2.0,1.0,0.0,2.0 },
		{ 1.0,-1.0,0.0,1.0 }
	};
	double u[4][4] = {0.0f}, v[3][3], c[4][3], d[3][4];
	double eps = 0.000001;
	if (sv_decomp(&a[0][0], 4, 3, &u[0][0], &v[0][0], eps, 5))
	{
/*
		printf("\n");
		printf("EXAMPLE(1)\n");
		printf("\n");
		printf("MAT U IS:\n");
		mat_printf(&u[0][0], 4, 4);
		printf("\n");
		printf("MAT V IS:\n");
		mat_printf(&v[0][0], 3, 3);
		printf("\n");
		printf("MAT A IS:\n");
		mat_printf(&a[0][0], 4, 3);
		printf("\n\n");
		printf("MAT UAV IS:\n");
		mm_mul(&u[0][0], &a[0][0], &c[0][0], 4, 4, 3);
		mm_mul(&c[0][0], &v[0][0], &a[0][0], 4, 3, 3);
		mat_printf(&a[0][0], 4, 3);
		printf("\n\n");
		printf("EXAMPLE(2)\n");
		printf("\n");
*/
	}
	/*
	if (sv_decomp(&b[0][0], 3, 4, &v[0][0], &u[0][0], eps, 5))
	{
		printf("MAT U IS:\n");
		mat_printf(&v[0][0], 3, 3);
		printf("\n");
		printf("MAT V IS:\n");
		mat_printf(&u[0][0], 4, 4);
		printf("\n");
		printf("MAT B IS:\n");
		mat_printf(&b[0][0], 3, 4);
		printf("\n\n");
		printf("MAT UBV IS:\n");
		mm_mul(&v[0][0], &b[0][0], &d[0][0], 3, 3, 4);
		mm_mul(&d[0][0], &u[0][0], &b[0][0], 3, 4, 4);
		mat_printf(&b[0][0], 3, 4);
		printf("\n");
	}
	*/
}
int main()
{
	test_gauss_jordan_solver();
	test_cholesky_solver();
	test_sv_decomp();
}