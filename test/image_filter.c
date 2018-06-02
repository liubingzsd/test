#include "file_wr.h"
#include "mat_math.h"
#include "image.h"
#include "image_filter.h"


void image_filter_1d_x_f32(image_s *src, image_s *dst, kernel_s *kernel, enum boundary_type type)
{
	uint16_t i, j;
	uint8_t j_ker, border;
	uint32_t *img_idx = NULL, x_idx;
	float tmp;
	float *src_buf = (float *)src->buf;
	float *dst_buf = (float *)dst->buf;
	float *kernel_buf = (float *)kernel->buf;
	img_idx = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size);
	border = (kernel->size - 1) >> 1;
	for (i = 0; i < src->rows; i++)
	{
		x_idx = i * src->cols;
		for (j = border; j < src->cols - border; j++)
		{
			tmp = 0;
			for (j_ker = 0; j_ker < kernel->size; j_ker++)
			{
				img_idx[j_ker] = x_idx + j + j_ker - border;
			}
			for (j_ker = 0; j_ker < kernel->size; j_ker++)
			{
				tmp += kernel_buf[j_ker] * src_buf[img_idx[j_ker]];
			}
			dst_buf[i * dst->cols + j] = tmp;
		}
	}
}

void image_filter_1d_y_f32(image_s *src, image_s *dst, kernel_s *kernel, enum boundary_type type)
{
	uint16_t i, j;
	uint8_t i_ker, border;
	uint32_t *img_idx = NULL, *x_idx = NULL;
	float tmp;
	float *src_buf = (float *)src->buf;
	float *dst_buf = (float *)dst->buf;
	float *kernel_buf = (float *)kernel->buf;
	img_idx = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size);
	x_idx = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size);
	border = (kernel->size - 1) >> 1;
	for (i = border; i < src->rows - border; i++)
	{
		for (i_ker = 0; i_ker < kernel->size; i_ker++)
		{
			x_idx[i_ker] = (i_ker + i - border) * src->cols;
		}

		for (j = 0; j < src->cols; j++)
		{
			tmp = 0;
			for (i_ker = 0; i_ker < kernel->size; i_ker++)
			{
				img_idx[i_ker] = x_idx[i_ker] + j;
			}
			for (i_ker = 0; i_ker < kernel->size; i_ker++)
			{
				tmp += kernel_buf[i_ker] * src_buf[img_idx[i_ker]];
			}
			dst_buf[i * dst->cols + j] = tmp;
		}
	}
}









void image_filter_1d_uint8_f32_x(image_s *src, image_s *dst, kernel_s *kernel, enum boundary_type type)
{
	uint16_t i, j;
	uint8_t j_ker, border;
	uint32_t *img_idx = NULL,x_idx;
	float tmp;
	uint8_t *src_buf = (uint8_t *)src->buf;
	uint8_t *dst_buf = (uint8_t *)dst->buf;
	float *kernel_buf = (float *)kernel->buf;
	img_idx = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size);
	border = (kernel->size - 1) >> 1;
	for (i = 0; i < src->rows; i++)
	{
		x_idx = i * src->cols;
		for (j = border; j < src->cols - border; j++)
		{
			tmp = 0;
			for (j_ker = 0; j_ker < kernel->size; j_ker++)
			{
				img_idx[j_ker] = x_idx + j + j_ker - border;
			}
			for (j_ker = 0; j_ker < kernel->size; j_ker++)
			{
				tmp += kernel_buf[j_ker] * src_buf[img_idx[j_ker]];
			}
			dst_buf[i * dst->cols + j] = (uint8_t)(tmp + 0.5);
		}
	}
}

void image_filter_1d_uint8_f32_y(image_s *src, image_s *dst, kernel_s *kernel, enum boundary_type type)
{
	uint16_t i, j;
	uint8_t i_ker, border;
	uint32_t *img_idx = NULL,*x_idx = NULL;
	float tmp;
	uint8_t *src_buf = (uint8_t *)src->buf;
	uint8_t *dst_buf = (uint8_t *)dst->buf;
	float *kernel_buf = (float *)kernel->buf;
	img_idx = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size);
	x_idx = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size);
	border = (kernel->size - 1) >> 1;
	for (i = border; i < src->rows - border; i++)
	{
		for (i_ker = 0; i_ker < kernel->size; i_ker++)
		{
			x_idx[i_ker] = (i_ker + i - border) * src->cols;
//			printf("________________idx = %d \n", i_ker + i - border);
		}
		
		for (j = 0; j < src->cols; j++)
		{
			tmp = 0;
			for (i_ker = 0; i_ker < kernel->size; i_ker++)
			{
				img_idx[i_ker] = x_idx[i_ker] + j;
			}
			for (i_ker = 0; i_ker < kernel->size; i_ker++)
			{
				tmp += kernel_buf[i_ker] * src_buf[img_idx[i_ker]];
//				printf("j = %d ker_buf = %f img_idx = %d  src_buf = %d  %f \n",j+1, kernel_buf[i_ker], img_idx[i_ker],src_buf[img_idx[i_ker]], tmp);
			}
			dst_buf[i * dst->cols + j] = (uint8_t)(tmp + 0.5f);
//			printf("dst_buf = %d \n", dst_buf[i * dst->cols + j]);
		}
	}
}


void image_filter_1d_x(uint8_t *src, uint8_t *dst, float *kernel, int src_size,int kernel_size, enum boundary_type type)
{
	uint16_t i, j;
	uint8_t i_ker, j_ker, border, idx;
	float tmp;
	uint8_t img_idx[3];
	border = (kernel_size - 1) >> 1;
	for (i = 0; i < src_size; i++)
	{
		idx = i * src_size;
		for (j = border; j < src_size - border; j++)
		{
			tmp = 0;
			for (j_ker = 0; j_ker < kernel_size; j_ker++)
			{
				img_idx[j_ker] = idx + j + j_ker - border;
			}
			for (i_ker = 0; i_ker < kernel_size; i_ker++)
			{
				tmp += kernel[i_ker] * src[img_idx[i_ker]];
			}
			dst[i * src_size + j] = (uint8_t)tmp;
		}
	}
}

void image_filter_1d_y(uint8_t *src1, uint8_t *dst1, float *kernel, int src_size, int kernel_size, enum boundary_type type)
{
	uint16_t i, j;
	uint8_t i_ker, j_ker, border, idx[3];
	float tmp;
	uint8_t img_idx[3];
	border = (kernel_size - 1) >> 1;
	for (i = border; i < src_size - border; i++)
	{
		idx[0] = (i - 1) * src_size;
		idx[1] = i * src_size;
		idx[2] = (i + 1) * src_size;
		for (j = 0; j < src_size; j++)
		{
			tmp = 0;
			for (j_ker = 0; j_ker < kernel_size; j_ker++)
			{
				img_idx[j_ker] = idx[j_ker] + j;
			}
			for (i_ker = 0; i_ker < kernel_size; i_ker++)
			{
				tmp += kernel[i_ker] * src1[img_idx[i_ker]];
			}
			dst1[i * src_size + j] = (uint8_t)tmp;
		}
	}
}
void test_image_filter()
{
	uint8_t src[6 * 6] = {
		15,20,25,25,15,10,
		20,15,50,30,20,15,
		20,50,55,60,30,20,
		20,15,65,30,15,30,
		15,20,30,20,25,30,
		20,25,15,20,10,15,
	};
	uint8_t dst[6 * 6] = {
		15,20,25,25,15,10,
		20,15,50,30,20,15,
		20,50,55,60,30,20,
		20,15,65,30,15,30,
		15,20,30,20,25,30,
		20,25,15,20,10,15,
	};

	uint8_t src1[6 * 6] = {
		15,20,24,23,16,10,
		20,25,36,33,21,15,
		20,44,55,51,35,20,
		20,29,44,35,22,30,
		15,21,25,24,25,30,
		20,21,19,16,14,15,
	};
	uint8_t dst1[6 * 6] = {
		15,20,24,23,16,10,
		20,25,36,33,21,15,
		20,44,55,51,35,20,
		20,29,44,35,22,30,
		15,21,25,24,25,30,
		20,21,19,16,14,15,
	};
	float kernel_1[3] = { 0.25,0.5,0.25 };
	int kernel_size = 7;
	float sigma = 1.0f;
	int m = 480;
	int n = 640;
	FILE *fp = fopen("data.dat", "r");
	image_s img_src, img_dst1,img_dst2;
	kernel_s kernel;
	image_create(&img_src, m, n, IMAGE_GRAYSCALE);
	image_create(&img_dst1, m, n, IMAGE_GRAYSCALE);
	image_create(&img_dst2, m, n, IMAGE_GRAYSCALE);
	kernel_create_1d(&kernel,kernel_size,KERNEL_F32);
	gauss_kernel_1d(&kernel,sigma);
	char str[] = "raw.dat";
	char str1[] = "x_filter.dat";
	char str2[] = "y_filter.dat";
	if (fp == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		read_data_from_file_uint8(fp, img_src.buf, m, n);
		fclose(fp);
	}
	image_filter_1d_uint8_f32_x(&img_src, &img_dst1, &kernel, NONE);
	write_image_data_to_file_uint8(str1, &img_dst1);
	image_filter_1d_uint8_f32_y(&img_dst1, &img_dst2, &kernel, NONE);
	write_image_data_to_file_uint8(str2,&img_dst2);

}