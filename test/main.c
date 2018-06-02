#include <stdio.h>
#include "gauss_jordan_solver.h"
#include "cholesky_solver.h"
#include "svd_solver.h"
#include "qr_solver.h"
#include "file_wr.h"
#include "ransac.h"
#include "image.h"
#include "sobel_filter.h"
#include "kernel.h"
#include "match.h"
#include "harris_corner.h"
#include "kd_tree.h"
#include "image_filter.h"
#include "mat.h"
int main()
{

//	test_gauss_jordan_solver();
//	test_cholesky_solver();
//	test_svd_cmp();
//	test_qr_solver();
//	test_svd_ginv();
//	test_svd_solver();
//	test_svd_ginv();
//	test_file_read_float();
//	test_ransac();
//	test((float *)a, 4, 3);
//	test_file_wr_malloc();
//	test_image();
//	test_sobel();
//	test_filter_kernel();
//  test_harris_corner();
//	test_sad();
//	test_kd_tree();
//	test_image_filter();
	test_mat();
	printf("**********  test module finished ***************\n");
	return 0;
}