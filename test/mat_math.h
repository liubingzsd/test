#ifndef _MAT_MATH_H_
#define _MAT__MATH_H_
#include <stdio.h>
#include <math.h>
void mm_mul(double *a, double *b, double *c, int m, int n, int k);
void mat_printf(double *a, int m, int n);
void vv_add(double *a, double *b, double *c, int length);
void sv_mul(double *a, int length, double s);
void vector_create(double *a, int length);
void vv_mul_mat(double *a, double *b, double *c, int m);
void mm_add(double *a, double *b, double *c, int m, int n);
void mm_sub(double *a, double *b, double *c, int m, int n);
void vector_printf(double *a, int length);
#endif
