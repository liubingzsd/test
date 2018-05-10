#ifndef _SVD_SOLVER_H_
#define _SVD_SOLVER_H_
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
bool svd_cmp(double *a, int m, int n, double *u, double *v, double eps, int ka);
void householder(double *a, int length, double *p);
void test_svd_cmp();
void test_svd_ginv();
void test_svd_solver();
#endif
