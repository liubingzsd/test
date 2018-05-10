#include <stdio.h>
#include "gauss_jordan_solver.h"
#include "cholesky_solver.h"
#include "svd_solver.h"
#include "qr_solver.h"

int main()
{
	test_gauss_jordan_solver();
	test_cholesky_solver();
	test_svd_cmp();
	test_qr_solver();
	test_svd_ginv();
	test_svd_solver();
}