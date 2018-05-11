#ifndef _QR_SOLVER_H_
#define _QR_SOLVER_H_
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
bool qr_cmp(double *a, int m, int n, double *q);
bool qr_solver(double *a,int m,int n,double *b,double *q);
void test_qr_solver();
#endif