#ifndef _MAT_MATH_H_
#define _MAT_MATH_H_
#include <stdio.h>
#include <math.h>
#include <stdint.h>


void mat_printf_uint8(uint8_t *mat, int m, int n);
void mm_mul(double *a, double *b, double *c, int m, int n, int k);
void mat_printf(double *a, int m, int n);
void vv_add(double *a, double *b, double *c, int length);
void sv_mul(double *a, int length, double s);
void vector_create(double *a, int length);
void vv_mul_mat(double *a, double *b, double *c, int m);
void mm_add(double *a, double *b, double *c, int m, int n);
void mm_sub(double *a, double *b, double *c, int m, int n);
void mm_sub_f32(float *a, float *b, float *c, int m, int n);
void vector_printf(double *a, int length);
void mv_mul(double *a, double *b, double *c, int m, int n);
void mat_printf_float(float *mat, int m, int n);

void mat_printf_uint16(uint16_t *mat, int m, int n);
void mat_printf_uint32(uint32_t *mat, int m, int n);

#endif
