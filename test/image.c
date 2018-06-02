#include <malloc.h>
#include "kernel.h"
#include "image.h"
#include "file_wr.h"

void write_image_data_to_file_uint8(char *str,image_s *img)
{
	FILE *fp1 = fopen(str, "w+");
	if (fp1 == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		write_data_to_file_uint8(fp1, img->buf, img->rows,img->cols);
	}
	fclose(fp1);
}
void write_image_data_to_file_int16(char *str, image_s *img)
{
	FILE *fp1 = fopen(str, "w+");
	if (fp1 == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		write_data_to_file_int16(fp1, img->buf, img->rows, img->cols);
	}
	fclose(fp1);
}

void write_image_data_to_file_float(char *str, image_s *img)
{
	FILE *fp1 = NULL;
	fp1 = fopen(str, "w+");
	if (fp1 == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		write_data_to_file_float(fp1, img->buf, img->rows, img->cols);
	}
	fclose(fp1);
}

void image_create(image_s *img, uint16_t rows, uint16_t cols, enum image_type type)
{
	img->type = type;
	img->rows = rows;
	img->cols = cols;

	if (type == IMAGE_YUV422) {
		img->buf_size = sizeof(uint8_t) * 2 * rows * cols;
	}
	else if (type == IMAGE_JPEG) {
		img->buf_size = sizeof(uint8_t) * 3 * rows * cols;
	}
	else if (type == IMAGE_GRADIENT) {
		img->buf_size = sizeof(int16_t) * rows * cols;
	}
	else if (type == IMAGE_GRAYSCALE) {
		img->buf_size = sizeof(uint8_t) * rows * cols;
	}
	else {
		img->buf_size = sizeof(float) * rows * cols;
	}
	img->buf = malloc(img->buf_size);
	if (img->buf)
	{
		//		printf("image buf allocated at 0x%x \n", (unsigned int)img->buf);
	}
	else
	{
		printf("___func___ image_create \n");
		printf("image buf allocated failed \n");
	}
	//	memcpy(img->buf,0,img->buf_size);
}

image_s *image_create_ptr(uint16_t rows, uint16_t cols, enum image_type type)
{
	image_s *img = NULL;
	img->type = type;
	img->rows = rows;
	img->cols = cols;

	if (type == IMAGE_YUV422) {
		img->buf_size = sizeof(uint8_t) * 2 * rows * cols;
	}
	else if (type == IMAGE_JPEG) {
		img->buf_size = sizeof(uint8_t) * 3 * rows * cols;
	}
	else if (type == IMAGE_GRADIENT) {
		img->buf_size = sizeof(int16_t) * rows * cols;
	}
	else if (type == IMAGE_GRAYSCALE) {
		img->buf_size = sizeof(uint8_t) * rows * cols;
	}
	else {
		img->buf_size = sizeof(float) * rows * cols;
	}
	img->buf = malloc(img->buf_size);
	if (img->buf)
	{
		//		printf("image buf allocated at 0x%x \n", (unsigned int)img->buf);
	}
	else
	{
		printf("___func___ image_create \n");
		printf("image buf allocated failed \n");
	}
	//	memcpy(img->buf,0,img->buf_size);
	return img;
}



void image_free(image_s *img)
{
	if (img->buf != NULL) {
		free(img->buf);
	}
	img->buf = NULL;
}

image_s *image_clone_f32(image_s *img)
{
	uint16_t i, j;
	uint32_t idx, x_idx;
	image_s *img_tmp;
	img_tmp = image_create_ptr(img->rows, img->cols, img->type);
	float *img_buf = (float *)img->buf;
	float *img_tmp_buf = (float *)img_tmp->buf;
	for (i = 0; i < img->rows; i++)
	{
		x_idx = i * img->cols;
		for (j = 0; j < img->cols; j++)
		{
			idx = x_idx + j;
			img_tmp_buf[idx] = img_buf[idx];
		}
	}
	return img_tmp;
}

image_s *image_clone_uint8(image_s *img)
{
	uint16_t i, j;
	uint32_t idx, x_idx;
	image_s *img_tmp;
	img_tmp = image_create_ptr(img->rows, img->cols, img->type);
	uint8_t *img_buf = (uint8_t *)img->buf;
	uint8_t *img_tmp_buf = (uint8_t *)img_tmp->buf;
	for (i = 0; i < img->rows; i++)
	{
		x_idx = i * img->cols;
		for (j = 0; j < img->cols; j++)
		{
			idx = x_idx + j;
			img_tmp_buf[idx] = img_buf[idx];
		}
	}
	return img_tmp;
}

void image_sub(image_s *img1,image_s *img2,image_s *img3)
{
	uint16_t i, j;
	uint32_t idx, x_idx;
	float tmp;
	if (img1->type != img2->type || img1->rows != img2->rows || img1->rows != img3->rows
		|| img1->cols != img2->cols || img1->cols != img3->cols)
	{
		printf("img1 type is not same to img2 type\n");
	}
	else
	{
		if (img1->type == IMAGE_F32)
		{
			float *img1_buf = (float *)img1->buf;
			float *img2_buf = (float *)img2->buf;
			float *img3_buf = (float *)img3->buf;
			for (i = 0; i < img3->rows; i++)
			{
				x_idx = i * img3->cols;
				for (j = 0; j < img3->cols; j++)
				{
					idx = x_idx + j;
					tmp = img1_buf[idx] - img2_buf[idx];
					if (tmp < 0)
					{
						tmp = 0;
					}
					img3_buf[idx] = tmp;
				}
			}
		}
		if (img1->type == IMAGE_GRAYSCALE)
		{
			uint8_t *img1_buf = (uint8_t *)img1->buf;
			uint8_t *img2_buf = (uint8_t *)img2->buf;
			uint8_t *img3_buf = (uint8_t *)img3->buf;
			for (i = 0; i < img3->rows; i++)
			{
				x_idx = i * img3->cols;
				for (j = 0; j < img3->cols; j++)
				{
					idx = x_idx + j;
					img3_buf[idx] = img1_buf[idx] - img2_buf[idx];
				}
			}
		}
	}
}



void image_conv(image_s *img,image_s *img1,int8_t *ker,int ker_size)
{
	int i, j, tmp;
	int ix1, ix2, ix3;
	int add[9] = {0};
	uint8_t *img_buf = (uint8_t *)img->buf;
	int16_t *img1_buf = (int16_t *)img1->buf;
	for (i = 1; i < img->rows - 1; i++)
	{
		ix1 = (i - 1)*img->cols;
		ix2 = i * img->cols;
		ix3 = (i - 1)*img->cols;
		tmp = 0;
		for (j = 1; j < img->cols - 1; j++)
		{
			add[0] = ix1 + j - 1;
			add[1] = ix1 + j;
			add[2] = ix1 + j + 1;
			add[3] = ix2 + j - 1;
			add[4] = ix2 + j;
			add[5] = ix2 + j + 1;
			add[6] = ix3 + j - 1;
			add[7] = ix3 + j;
			add[8] = ix3 + j + 1;
			tmp += ker[0] * img_buf[add[0]] + ker[1] * img_buf[add[1]] + ker[2] * img_buf[add[2]];
			tmp += ker[3] * img_buf[add[3]] + ker[4] * img_buf[add[4]] + ker[5] * img_buf[add[5]];
			tmp += ker[6] * img_buf[add[6]] + ker[7] * img_buf[add[7]] + ker[8] * img_buf[add[8]];
			img1_buf[(i - 1)*img1->cols + (j - 1)] = tmp;
		}
	}
}

void image_conv_uint8_int16(image_s *img, image_s *img1, int8_t *ker, uint8_t ker_size)
{
	int i, j, tmp;
	uint8_t i_ker,j_ker,border,idx;
	uint32_t *ix = NULL,*img_index = NULL;
	uint8_t *img_buf = (uint8_t *)img->buf;
	int16_t *img1_buf = (int16_t *)img1->buf;
	img_index = (uint32_t *)malloc(sizeof(uint32_t) * ker_size * ker_size);
	border = (ker_size - 1) / 2;
	if (!img_index)
	{
		printf("_______func image_conv_________ \n");
		printf("img_index memory allocated failed \n");
	}
	ix = (uint32_t *)malloc(sizeof(uint32_t) * ker_size );
	if (!ix)
	{
		printf("_______fun image_conv_________ \n");
		printf("ix memory allocated failed \n");
	}

	for (i = border; i < img->rows - border; i++)
	{
		for (i_ker = 0; i_ker < ker_size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->cols;
		}

		for (j = border; j < img->cols - border; j++)
		{
			tmp = 0;
			for (i_ker = 0; i_ker < ker_size; i_ker++)
			{
				for (j_ker = 0; j_ker < ker_size; j_ker++)
				{
					img_index[i_ker*ker_size + j_ker] = ix[i_ker] + j + j_ker - border;
				}
			}
			for (i_ker = 0; i_ker < ker_size; i_ker++)
			{
				for (j_ker = 0; j_ker < ker_size; j_ker++)
				{
					idx = i_ker * ker_size + j_ker;
					tmp += ker[idx] * img_buf[img_index[idx]];
				}
			}
			img1_buf[(i - border)*img1->cols + (j - border)] = tmp;
		}
	}
}

void image_conv_boarder_uint8_int16(image_s *img, image_s *img1, int8_t *ker, uint8_t ker_size)
{
	int i, j, tmp;
	uint8_t i_ker, j_ker, border, idx;
	uint32_t *ix = NULL, *img_index = NULL;
	uint8_t *img_buf = (uint8_t *)img->buf;
	int16_t *img1_buf = (int16_t *)img1->buf;
	img_index = (uint32_t *)malloc(sizeof(uint32_t) * ker_size * ker_size);
	border = (ker_size - 1) / 2;
	if (!img_index)
	{
		printf("_______func image_conv_________ \n");
		printf("img_index memory allocated failed \n");
	}
	ix = (uint32_t *)malloc(sizeof(uint32_t) * ker_size);
	if (!ix)
	{
		printf("_______fun image_conv_________ \n");
		printf("ix memory allocated failed \n");
	}

	for (i = border; i < img->rows - border; i++)
	{
		for (i_ker = 0; i_ker < ker_size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->cols;
		}

		for (j = border; j < img->cols - border; j++)
		{
			tmp = 0;
			for (i_ker = 0; i_ker < ker_size; i_ker++)
			{
				for (j_ker = 0; j_ker < ker_size; j_ker++)
				{
					img_index[i_ker*ker_size + j_ker] = ix[i_ker] + j + j_ker - border;
				}
			}
			for (i_ker = 0; i_ker < ker_size; i_ker++)
			{
				for (j_ker = 0; j_ker < ker_size; j_ker++)
				{
					idx = i_ker * ker_size + j_ker;
					tmp += ker[idx] * img_buf[img_index[idx]];
				}
			}
			img1_buf[(i - border)*img1->cols + (j - border)] = tmp;
		}
	}
}



void image_conv_int16_f32(image_s *img, image_s *img1, float *ker, uint8_t ker_size)
{
	int i, j;
	float tmp;
	uint8_t i_ker, j_ker, border, idx;
	uint32_t *ix = NULL, *add = NULL;
	uint8_t *img_buf = (uint8_t *)img->buf;
	float *img1_buf = (float *)img1->buf;
	add = (uint32_t *)malloc(sizeof(uint32_t) * ker_size * ker_size);
	border = (ker_size - 1) >> 1;
	if (!add)
	{
		printf("_______func image_conv_________ \n");
		printf("add memory allocated failed \n");
	}
	ix = (uint32_t *)malloc(sizeof(uint32_t) * ker_size);
	if (!ix)
	{
		printf("_______fun image_conv_________ \n");
		printf("ix memory allocated failed \n");
	}

	for (i = border; i < img->rows - border; i++)
	{
		for (i_ker = 0; i_ker < ker_size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->cols;
		}
		for (j = border; j < img->cols - border; j++)
		{
			tmp = 0;
			for (i_ker = 0; i_ker < ker_size; i_ker++)
			{
				for (j_ker = 0; j_ker < ker_size; j_ker++)
				{
					add[i_ker*ker_size + j_ker] = ix[i_ker] + j + j_ker - border;
				}
			}
			for (i_ker = 0; i_ker < ker_size; i_ker++)
			{
				for (j_ker = 0; j_ker < ker_size; j_ker++)
				{
					idx = i_ker * ker_size + j_ker;
					tmp += ker[idx] * img_buf[add[idx]];
				}
			}
			img1_buf[(i - border)*img1->cols + (j - border)] = tmp;
		}
	}
}

void image_conv_kernel_int16_f32(image_s *img, image_s *img1, kernel_s *kernel)
{
	int i, j;
	float tmp;
	uint8_t i_ker, j_ker, border, idx;
	uint32_t *ix = NULL, *add = NULL;
	int16_t *img_buf = (int16_t *)img->buf;
	float *img1_buf = (float *)img1->buf;
	float *kernel_buf = (float *)kernel->buf;
	add = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size * kernel->size);
	border = (kernel->size - 1) >> 1;
	if (!add)
	{
		printf("_______func image_conv_________ \n");
		printf("add memory allocated failed \n");
	}
	ix = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size);
	if (!ix)
	{
		printf("_______fun image_conv_________ \n");
		printf("ix memory allocated failed \n");
	}

	for (i = border; i < img->rows - border; i++)
	{
		for (i_ker = 0; i_ker < kernel->size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->cols;
		}

		for (j = border; j < img->cols - border; j++)
		{
			tmp = 0;
			for (i_ker = 0; i_ker < kernel->size; i_ker++)
			{
				for (j_ker = 0; j_ker < kernel->size; j_ker++)
				{
					add[i_ker*kernel->size + j_ker] = ix[i_ker] + j + j_ker - border;
				}
			}
			for (i_ker = 0; i_ker < kernel->size; i_ker++)
			{
				for (j_ker = 0; j_ker < kernel->size; j_ker++)
				{
					idx = i_ker * kernel->size + j_ker;
					tmp += kernel_buf[idx] * img_buf[add[idx]];
				}
			}
			img1_buf[(i - border)*img1->cols + (j - border)] = tmp;
		}
	}
}

void image_conv_kernel_f32_f32(image_s *img, image_s *img1, kernel_s *kernel)
{
	int i, j;
	float tmp;
	uint8_t i_ker, j_ker, border, idx;
	uint32_t *ix = NULL, *add = NULL;
	float *img_buf = (float *)img->buf;
	float *img1_buf = (float *)img1->buf;
	float *kernel_buf = (float *)kernel->buf;
	add = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size * kernel->size);
	border = (kernel->size - 1) >> 1;
	if (!add)
	{
		printf("_______func image_conv_________ \n");
		printf("add memory allocated failed \n");
	}
	ix = (uint32_t *)malloc(sizeof(uint32_t) * kernel->size);
	if (!ix)
	{
		printf("_______fun image_conv_________ \n");
		printf("ix memory allocated failed \n");
	}

	for (i = border; i < img->rows - border; i++)
	{
		for (i_ker = 0; i_ker < kernel->size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->cols;
		}

		for (j = border; j < img->cols - border; j++)
		{
			tmp = 0;
			for (i_ker = 0; i_ker < kernel->size; i_ker++)
			{
				for (j_ker = 0; j_ker < kernel->size; j_ker++)
				{
					add[i_ker*kernel->size + j_ker] = ix[i_ker] + j + j_ker - border;
				}
			}
			for (i_ker = 0; i_ker < kernel->size; i_ker++)
			{
				for (j_ker = 0; j_ker < kernel->size; j_ker++)
				{
					idx = i_ker * kernel->size + j_ker;
					tmp += kernel_buf[idx] * img_buf[add[idx]];
				}
			}
			img1_buf[(i - border)*img1->cols + (j - border)] = tmp;
		}
	}
}

void image_resize_double_f32(image_s *src,image_s *dst)
{
	uint16_t i, j;
	uint32_t x_idx, idx;
	float *src_buf = (float *)src->buf;
	float *dst_buf = (float *)dst->buf;
	if (dst->rows != 2 * src->rows || dst->cols != 2 * src->cols)
	{
		printf("reconfirm the size of dst_image and src_image \n");
	}
	else
	{
		for (i = 0; i < dst->rows; i++)
		{
			x_idx = i * dst->cols;
			for (j = 0; j < dst->cols; j++)
			{
				idx = x_idx + j;
				dst_buf[idx] = src_buf[(i / 2)*src->cols + (j / 2)];
			}
		}
		for (i = 0; i < dst->rows; i += 2)
		{
			x_idx = i * dst->cols;
			for (j = 1; j < dst->cols - 1; j += 2)
			{
				idx = x_idx + j;
				dst_buf[idx] = 0.5f * (src_buf[(i / 2)*src->cols + (j / 2)] + (src_buf[(i / 2)*src->cols + (j / 2) + 1]));
			}
		}

		for (i = 1; i < dst->rows-1; i += 2)
		{
			x_idx = i * dst->cols;
			for (j = 0; j < dst->cols; j += 2)
			{
				idx = x_idx + j;
				dst_buf[idx] = 0.5f * (src_buf[(i / 2)*src->cols + (j / 2)] + (src_buf[((i / 2) + 1)*src->cols + (j / 2)]));
			}
		}

		for (i = 1; i < dst->rows - 1; i += 2)
		{
			x_idx = i * dst->cols;
			for (j = 0; j < dst->cols - 1; j += 2)
			{
				idx = x_idx + j;
				dst_buf[idx] = 0.25f * (src_buf[(i / 2)*src->cols + (j / 2)] + (src_buf[(i / 2) *src->cols + (j / 2)]));
			}
		}
	}
}


void image_convert_u8_f32(image_s *src,image_s *dst)
{
	uint8_t *src_buf = (uint8_t *)src->buf;
	float *dst_buf = (float *)dst->buf;
	uint16_t i, j;
	uint32_t x_idx,idx;
	if (src->rows != dst->rows || src->cols != dst->cols)
	{
		printf("src_image is not equal to dst_image \n");
	}
	else
	{
		for (i = 0; i < dst->rows; i++)
		{
			x_idx = i * dst->cols;
			for (j = 0; j < dst->cols; j++)
			{
				idx = x_idx + j;
				dst_buf[idx] = src_buf[idx];
			}
		}
	}

}
void image_convert_f32_scale(image_s *src,image_s *dst,float scale)
{
	float *src_buf = (float *)src->buf;
	float *dst_buf = (float *)dst->buf;
	uint16_t i, j;
	uint32_t x_idx, idx;
	if (src->rows != dst->rows || src->cols != dst->cols)
	{
		printf("src_image is not equal to dst_image \n");
	}
	else
	{
		for (i = 0; i < dst->rows; i++)
		{
			x_idx = i * dst->cols;
			for (j = 0; j < dst->cols; j++)
			{
				idx = x_idx + j;
				dst_buf[idx] = src_buf[idx] * scale;
			}
		}
	}
}

image_s *image_resize_half_f32(image_s *src)
{
	uint16_t i, j;
	uint16_t row, col;
	row = src->rows / 2;
	col = src->cols / 2;
	uint32_t idx, x_idx;
	image_s *img_new = image_create_ptr(row, col, IMAGE_F32);
	float *img_new_buf = (float *)img_new->buf;
	float *src_buf = (float *)src->buf;
	for (i = 0; i < row; i++)
	{
		x_idx = i * img_new->cols;
		for (j = 0; j < col; j++)
		{
			idx = x_idx + j;
			img_new_buf[idx] = src_buf[(2 * i)*src->cols + (2 * j)];
		}
	}
	return img_new;
}


void test_image()
{
	int m = 480;
	int n = 640;
	image_s image,image1,image2;
	image_create(&image,m,n,IMAGE_GRAYSCALE);
	image_create(&image1, m-1, n-1, IMAGE_GRADIENT);
	image_create(&image2, m-1, n-1, IMAGE_GRADIENT);
	FILE *fp = fopen("data.dat", "r");
	char str[] = "image1.dat";
	char str1[] = "dx.dat";
	char str2[] = "dy.dat";
	if (fp == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		read_data_from_file_uint8(fp,image.buf,m,n);
	}
	fclose(fp);
	write_image_data_to_file_uint8(str, &image);
	write_image_data_to_file_int16(str1, &image1);
	write_image_data_to_file_int16(str2, &image2);
	image_free(&image);
	image_free(&image1);
	image_free(&image2);
}
