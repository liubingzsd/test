#include "file_wr.h"
#include "sobel_filter.h"
#include "image.h"
int8_t gx[9] = { -1,0,1,-2,0,2,-1,0,1 };
int8_t gy[9] = { -1,-2,-1,0,0,0,1,2,1 };

void calc_sobel_gx(image_t *img,image_t *img1,int8_t *kernel)
{
	int x, y;
	int ix1, ix2, ix3;
	int t0, t1, t2, t3, t4, t5;
	int tmp;
	uint8_t *img_buf = (uint8_t *)img->buf;
	int16_t *img1_buf = (int16_t *)img1->buf;
	for (x = 1; x < img->w - 1; x++)
	{
		ix1 = (x - 1)*img->h;
		ix2 = x * img->h;
		ix3 = (x + 1)*img->h;
		for (y = 1; y < img->h - 1; y++)
		{
			t0 = ix1 + y - 1;
			t1 = ix2 + y - 1;
			t2 = ix3 + y - 1;
			t3 = ix1 + y + 1;
			t4 = ix2 + y + 1;
			t5 = ix3 + y + 1;
			tmp = -img_buf[t0] - 2 * img_buf[t1] - img_buf[t2] + img_buf[t3] + 2 * img_buf[t4] + img_buf[t5];
			if (tmp < 0)
			{
				img1_buf[(x - 1)*img1->h + (y - 1)] = -tmp;
			}
			else
			{
				img1_buf[(x - 1)*img1->h + (y - 1)] = tmp;
			}

		}
	}
}
void calc_sobel_gy(image_t *img, image_t *img1,int8_t *kernel)
{
	int x, y;
	int ix1, ix2;
	int t0, t1, t2, t3, t4, t5;
	int tmp;
	uint8_t *img_buf = (uint8_t *)img->buf;
	int16_t *img1_buf = (int16_t *)img1->buf;
	for (x = 1; x < img->w - 1; x++)
	{
		ix1 = (x - 1)*img->h;
		ix2 = (x + 1)*img->h;
		for (y = 1; y < img->h - 1; y++)
		{
			t0 = ix1 + y - 1;
			t1 = t0 + 1;
			t2 = t0 + 2;
			t3 = ix2 + y - 1;
			t4 = t3 + 1;
			t5 = t3 + 2;
			tmp = -img_buf[t0] - 2 * img_buf[t1] - img_buf[t2] + img_buf[t3] + 2 * img_buf[t4] + img_buf[t5];
			if (tmp < 0)
			{
				img1_buf[(x - 1)*img1->h + (y - 1)] = -tmp;
			}
			else
			{
				img1_buf[(x - 1)*img1->h + (y - 1)] = tmp;
			}

		}
	}
}
void calc_sobel_g(image_t *dx,image_t *dy, image_t *dst)
{
	int i, j;
	int16_t *dx_buf = (int16_t *)dx->buf;
	int16_t *dy_buf = (int16_t *)dy->buf;
	uint8_t *dst_buf = (uint8_t *)dst->buf;
	int16_t tmp = 0;
	for (i = 0; i < dst->w; i++)
	{
		for (j = 0; j < dst->h; j++)
		{
			tmp = dx_buf[i*dx->h + j] + dy_buf[i*dy->h + j];
			if (tmp > 255)
			{
				tmp = 255;
			}
			dst_buf[i*dst->h + j] = (uint8_t)tmp;
		}
	}
}
void sobel_filter(image_t *img,image_t *dst,int8_t *x_kernel,int8_t *y_kernel)
{
	image_t image_dx, image_dy;
	image_create(&image_dx, img->w - 2, img->h - 2, IMAGE_GRADIENT);
	image_create(&image_dy, img->w - 2, img->h - 2, IMAGE_GRADIENT);
	calc_sobel_gx(img, &image_dx, x_kernel);
	calc_sobel_gy(img, &image_dy, y_kernel);
	calc_sobel_g(&image_dx,&image_dy,dst);
	image_free(&image_dx);
	image_free(&image_dy);
}

void test_sobel()
{
	printf("test sobel filter\n");
	int m = 480;
	int n = 640;
	image_t image, image_dx, image_dy,dst;
	image_create(&image, m, n, IMAGE_GRAYSCALE);
	image_create(&dst, m - 2, n - 2, IMAGE_GRAYSCALE);
	image_create(&image_dx, m - 2, n - 2, IMAGE_GRADIENT);
	image_create(&image_dy, m - 2, n - 2, IMAGE_GRADIENT);
	FILE *fp = fopen("data.dat", "r");
	char str[] = "image1.dat";
	char str1[] = "sobel_filter.dat";
	if (fp == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		read_data_from_file_uint8(fp, image.buf, m, n);
	}
	fclose(fp);
	sobel_filter(&image, &dst, gx, gy);
	write_image_data_to_file_uint8(str1,&dst);
	image_free(&image);
	image_free(&dst);
}


