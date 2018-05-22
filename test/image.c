#include <malloc.h>
#include "kernel.h"
#include "image.h"
#include "file_wr.h"

void write_image_data_to_file_uint8(char *str,image_t *img)
{
	FILE *fp1 = fopen(str, "w+");
	if (fp1 == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		write_data_to_file_uint8(fp1, img->buf, img->w,img->h);
	}
	fclose(fp1);
}
void write_image_data_to_file_int16(char *str, image_t *img)
{
	FILE *fp1 = fopen(str, "w+");
	if (fp1 == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		write_data_to_file_int16(fp1, img->buf, img->w, img->h);
	}
	fclose(fp1);
}

void write_image_data_to_file_float(char *str, image_t *img)
{
	FILE *fp1 = NULL;
	fp1 = fopen(str, "w+");
	if (fp1 == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		write_data_to_file_float(fp1, img->buf, img->w, img->h);
	}
	fclose(fp1);
}

void image_create(image_t *img, uint16_t width, uint16_t height, enum image_type type)
{
	img->type = type;
	img->w = width;
	img->h = height;

	if (type == IMAGE_YUV422) {
		img->buf_size = sizeof(uint8_t) * 2 * width * height;
	}
	else if (type == IMAGE_JPEG) {
		img->buf_size = sizeof(uint8_t) * 3 * width * height; 
	}
	else if (type == IMAGE_GRADIENT) {
		img->buf_size = sizeof(int16_t) * width * height;
	}
	else if(type == IMAGE_GRAYSCALE){
		img->buf_size = sizeof(uint8_t) * width * height;
	}else{
		img->buf_size = sizeof(float) * width * height;
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

void image_free(image_t *img)
{
	if (img->buf != NULL) {
		free(img->buf);
	}
	img->buf = NULL;
}

int image_calc_kernel_int(image_t *image,kernel_t *kernel,int idx,int jdx)
{
	int tmp = 0;

	return tmp;
}

float image_calc_kernel_float(image_t *image, kernel_t *kernel,int idx, int jdx)
{
	float tmp = 0;
	return tmp;
}

void image_conv(image_t *img,image_t *img1,int8_t *ker,int ker_size)
{
	int i, j, tmp;
	int ix1, ix2, ix3;
	int add[9] = {0};
	uint8_t *img_buf = (uint8_t *)img->buf;
	int16_t *img1_buf = (int16_t *)img1->buf;
	for (i = 1; i < img->w - 1; i++)
	{
		ix1 = (i - 1)*img->h;
		ix2 = i * img->h;
		ix3 = (i - 1)*img->h;
		tmp = 0;
		for (j = 1; j < img->h - 1; j++)
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
			img1_buf[(i - 1)*img1->h + (j - 1)] = tmp;
		}
	}
}

void image_conv_uint8_int16(image_t *img, image_t *img1, int8_t *ker, uint8_t ker_size)
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

	for (i = border; i < img->w - border; i++)
	{
		for (i_ker = 0; i_ker < ker_size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->h;
		}

		for (j = border; j < img->h - border; j++)
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
			img1_buf[(i - border)*img1->h + (j - border)] = tmp;
		}
	}
}

void image_conv_boarder_uint8_int16(image_t *img, image_t *img1, int8_t *ker, uint8_t ker_size)
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

	for (i = border; i < img->w - border; i++)
	{
		for (i_ker = 0; i_ker < ker_size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->h;
		}

		for (j = border; j < img->h - border; j++)
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
			img1_buf[(i - border)*img1->h + (j - border)] = tmp;
		}
	}
}



void image_conv_int16_f32(image_t *img, image_t *img1, float *ker, uint8_t ker_size)
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

	for (i = border; i < img->w - border; i++)
	{
		for (i_ker = 0; i_ker < ker_size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->h;
		}
		for (j = border; j < img->h - border; j++)
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
			img1_buf[(i - border)*img1->h + (j - border)] = tmp;
		}
	}
}

void image_conv_kernel_int16_f32(image_t *img, image_t *img1, kernel_t *kernel)
{
	int i, j;
	float tmp;
	uint8_t i_ker, j_ker, border, idx;
	uint32_t *ix = NULL, *add = NULL;
	int16_t *img_buf = (int16_t *)img->buf;
	float *img1_buf = (float *)img1->buf;
	float *kernel_buf = (float *)kernel->buf;
	add = (float *)malloc(sizeof(float) * kernel->size * kernel->size);
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

	for (i = border; i < img->w - border; i++)
	{
		for (i_ker = 0; i_ker < kernel->size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->h;
		}

		for (j = border; j < img->h - border; j++)
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
			img1_buf[(i - border)*img1->h + (j - border)] = tmp;
		}
	}
}

void image_conv_kernel_f32_f32(image_t *img, image_t *img1, kernel_t *kernel)
{
	int i, j;
	float tmp;
	uint8_t i_ker, j_ker, border, idx;
	uint32_t *ix = NULL, *add = NULL;
	float *img_buf = (float *)img->buf;
	float *img1_buf = (float *)img1->buf;
	float *kernel_buf = (float *)kernel->buf;
	add = (float *)malloc(sizeof(float) * kernel->size * kernel->size);
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

	for (i = border; i < img->w - border; i++)
	{
		for (i_ker = 0; i_ker < kernel->size; i_ker++)
		{
			ix[i_ker] = (i + i_ker - border)*img->h;
		}

		for (j = border; j < img->h - border; j++)
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
			img1_buf[(i - border)*img1->h + (j - border)] = tmp;
		}
	}
}


void test_image()
{
	int m = 480;
	int n = 640;
	image_t image,image1,image2;
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
}
