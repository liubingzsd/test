#include <time.h>
#include "image.h"
#include "kernel.h"
#include "feature_point.h"
#include "harris_corner.h"

int8_t fx[9] = {-1,0,1,-1,0,1,-1,0,1};
int8_t fy[9] = {-1,-1,-1,0,0,0,1,1,1};

void harris_corner_select(harris_corner_detector_t *detector,image_t *image)
{
	float *image_buf = (float *)image->buf;
	float tmp;
	float val[8];
	int i, j;
	for (i = 1; i < image->w - 1; i++)
	{
		for (j = 1; j < image->h - 1; j++)
		{
			tmp = image_buf[i*image->h + j];
			val[0] = image_buf[(i - 1)*image->h + j - 1];
			val[1] = image_buf[(i - 1)*image->h + j];
			val[2] = image_buf[(i - 1)*image->h + j + 1];
			val[3] = image_buf[i*image->h + j - 1];
			val[4] = image_buf[i*image->h + j + 1];
			val[5] = image_buf[(i + 1)*image->h + j - 1];
			val[6] = image_buf[(i + 1)*image->h + j];
			val[7] = image_buf[(i + 1)*image->h + j + 1];
			if (tmp > detector->thres * detector->response_max_val && tmp > val[0] && tmp > val[1] && tmp > val[2] && tmp > val[3] && tmp > val[4] && tmp > val[5] && tmp > val[6] && tmp > val[7])
			{
				detector->point.buf[2 * detector->point.num] = i;
				detector->point.buf[2 * detector->point.num + 1] = j;
				detector->point.num += 1;
				printf("%d %d \n", i + 5, j + 5);
			}
		}
	}
	printf("detector point num is %d \n", detector->point.num);
}

void det_of_mat(harris_corner_detector_t *detector,image_t *dxx_g, image_t *dyy_g, image_t *dxy_g)
{
	image_t r;
	int i, j, idx;
	float a, b, c;
	float *dxx_g_buf = (float *)dxx_g->buf;
	float *dxy_g_buf = (float *)dxy_g->buf;
	float *dyy_g_buf = (float *)dyy_g->buf;
	char str[] = "r.dat";
	image_create(&r, dxx_g->w, dxx_g->h, IMAGE_F32);
	float *r_buf = (float *)r.buf;
	for (i = 0; i < r.w; i++)
	{
		for (j = 0; j < r.h; j++)
		{
			idx = i * r.h + j;
			a = dxx_g_buf[idx];
			b = dxy_g_buf[idx];
			c = dyy_g_buf[idx];
			r_buf[idx] = a * c - b * b - detector->k*(a + c)*(a + c);
			if (r_buf[idx] > detector->response_max_val)
			{
				detector->response_max_val = r_buf[idx];
			}
		}
	}
	write_image_data_to_file_float(str,&r);
	harris_corner_select(detector,&r);
	image_free(dxx_g);
	image_free(dxy_g);
	image_free(dyy_g);
	image_free(&r);
}



/**/
void harris_corner_calc_image_gradients(image_t *img,image_t *dx,image_t *dy,int8_t *kx,int8_t *ky,uint8_t size)
{
	image_conv_uint8_int16(img, dx, kx, size);
	image_conv_uint8_int16(img, dy, ky, size);
}

void harris_corner_calc_products_of_image_gradients(image_t *dx,image_t *dy,image_t *dxx,image_t *dyy,image_t *dxy)
{
	char str1[] = "dxx.dat";
	char str2[] = "dxy.dat";
	char str3[] = "dyy.dat";
	int i, j;
	int idx;
	int16_t *dx_buf = (int16_t *)dx->buf;
	int16_t *dy_buf = (int16_t *)dy->buf;
	float *dxx_buf = (float *)dxx->buf;
	float *dyy_buf = (float *)dyy->buf;
	float *dxy_buf = (float *)dxy->buf;
	for (i = 0; i < dx->w; i++)
	{
		for (j = 0; j < dx->h; j++)
		{
			idx = i * dx->h + j;
			dxx_buf[idx] = dx_buf[idx] * dx_buf[idx];
			dyy_buf[idx] = dy_buf[idx] * dy_buf[idx];
			dxy_buf[idx] = dx_buf[idx] * dy_buf[idx];
		}
	}
//	write_image_data_to_file_float(str1, dxx);
//	write_image_data_to_file_float(str2, dxy);
//	write_image_data_to_file_float(str3, dyy);
	image_free(dx);
	image_free(dy);

}

void harris_corner_gauss_filter(harris_corner_detector_t *detector,image_t *dxx,image_t *dyy,image_t *dxy,kernel_t *kernel)
{
	uint8_t border = kernel->size - 1;
	image_t dxx_g, dyy_g, dxy_g;
	image_create(&dxx_g, dxx->w - border, dxx->h - border, IMAGE_F32);
	image_create(&dyy_g, dyy->w - border, dyy->h - border, IMAGE_F32);
	image_create(&dxy_g, dxy->w - border, dxy->h - border, IMAGE_F32);
	image_conv_kernel_f32_f32(dxx, &dxx_g, kernel);
	image_conv_kernel_f32_f32(dyy, &dyy_g, kernel);
	image_conv_kernel_f32_f32(dxy, &dxy_g, kernel);
	det_of_mat(detector,&dxx_g, &dyy_g, &dxy_g);
	image_free(dxx);
	image_free(dyy);
	image_free(dxy);
}


void harris_corner(harris_corner_detector_t *detector,image_t *img,int8_t *kx,int8_t *ky,uint8_t size,kernel_t *kernel)
{
	uint8_t border = size - 1;
	image_t dx, dy, dxx, dyy, dxy;
	image_create(&dx, img->w - border, img->h - border, IMAGE_GRADIENT);
	image_create(&dy, img->w - border, img->h - border, IMAGE_GRADIENT);
	image_create(&dxx, img->w - border, img->h - border, IMAGE_F32);
	image_create(&dyy, img->w - border, img->h - border, IMAGE_F32);
	image_create(&dxy, img->w - border, img->h - border, IMAGE_F32);
	harris_corner_calc_image_gradients(img,&dx,&dy,kx,ky,size);
	harris_corner_calc_products_of_image_gradients(&dx, &dy, &dxx, &dyy, &dxy);
	harris_corner_gauss_filter(detector,&dxx,&dyy,&dxy,kernel);
}

void harris_corner_detector_init(harris_corner_detector_t *detector)
{
	detector->point.num = 0;
	detector->thres = 0.01;
	detector->response_max_val = 0;
	detector->k = 0.06;
}


void test_harris_corner()
{
	int i, test_time = 10;
	int m = 480;
	int n = 640;
	kernel_t kernel;
	harris_corner_detector_t detector;
	uint8_t kernel_size = 7;
	float sigma = 1.0f;
	image_t image;
	image_create(&image, m, n, IMAGE_GRAYSCALE);
	FILE *fp = NULL;
	fp = fopen("data.dat", "r");
	if (fp == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		read_data_from_file_uint8(fp, image.buf, m, n);
	}
	fclose(fp);

	kernel_create(&kernel, kernel_size, KERNEL_F32);
	gauss_kernel_2d(&kernel, sigma, sigma);
	for (i = 0; i < 10; i++)
	{
		harris_corner_detector_init(&detector);
		clock_t start_time = clock();
		harris_corner(&detector,&image, fx, fy, 3, &kernel);
		clock_t end_time = clock();
		printf("time = %f \n", (float)(end_time - start_time) / CLOCKS_PER_SEC * 1000);
	}
	image_free(&image);
	kernel_free(&kernel);
}