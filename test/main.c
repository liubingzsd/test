#include <stdio.h>
#include "gauss_jordan_solver.h"
int main()
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