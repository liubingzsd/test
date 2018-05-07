#ifndef _SV_DECOMP_H_
#define _SV_DECOMP_H_
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
bool sv_decomp(double *a, int m, int n, double *u, double *v, double eps, int ka);
#endif
