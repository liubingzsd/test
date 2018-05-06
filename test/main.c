#include <stdio.h>
#include "gauss_jordan_solver.h"
#include "cholesky_solver.h"
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
int main()
{
	test_gauss_jordan_solver();
	test_cholesky_solver();
}