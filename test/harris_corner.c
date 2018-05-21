#include "image.h"
#include "kernel.h"
#include "harris_corner.h"

int8_t fx[9] = {-1,0,1,-1,0,1,-1,0,1};
int8_t fy[9] = {-1,-1,-1,0,0,0,1,1,1};


float r_max = 0;
/**/
typedef struct
{
	uint32_t num;
	uint32_t buf_size;
	uint32_t *buf;
}feature_point_t;

void feature_point_create(feature_point_t *point,int m,int n)
{
	point->buf_size = sizeof(uint32_t) * m * n * 2;
	point->buf = malloc(point->buf_size);
	point->num = 0;
	if (point->buf)
	{
//		printf("memory allocated at 0x%x \n", point->buf);
	}
	else
	{
		printf("___func___ feature_point_create \n");
		printf("memory allocated failed \n");
	}
}
void feature_point_free(feature_point_t *point)
{
	if (point->buf)
	{
		free(point->buf);
	}
	point->buf = NULL;
}
void printf_feature_point_to_file(feature_point_t *point)
{

}

void harris_corner_select(image_t *image, feature_point_t *point, float thres)
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
			if (tmp > thres * r_max && tmp > val[0] && tmp > val[1] && tmp > val[2] && tmp > val[3] && tmp > val[4] && tmp > val[5] && tmp > val[6] && tmp > val[7])
			{
				point->buf[point->num] = i;
				point->buf[point->num + 1] = j;
				point->num += 2;
				printf("%d %d \n", i + 5, j + 5);
			}
		}
	}
	printf("point num is %d \n", point->num/2);
}

void det_of_mat(image_t *dxx_g, image_t *dyy_g, image_t *dxy_g, feature_point_t *point, float thres)
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
			r_buf[idx] = a * c - b * b - 0.06*(a + c)*(a + c);
			if (r_buf[idx] > r_max)
			{
				r_max = r_buf[idx];
			}
		}
	}
	write_image_data_to_file_float(str,&r);
	harris_corner_select(&r, point, thres);
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

void harris_corner_gauss_filter(image_t *dxx,image_t *dyy,image_t *dxy,kernel_t *kernel,feature_point_t *point,float thres)
{
	uint8_t border = kernel->size - 1;
	image_t dxx_g, dyy_g, dxy_g;
	char str1[] = "dxx_g.dat";
	char str2[] = "dxy_g.dat";
	char str3[] = "dyy_g.dat";
	image_create(&dxx_g, dxx->w - border, dxx->h - border, IMAGE_F32);
	image_create(&dyy_g, dyy->w - border, dyy->h - border, IMAGE_F32);
	image_create(&dxy_g, dxy->w - border, dxy->h - border, IMAGE_F32);
	image_conv_kernel_f32_f32(dxx, &dxx_g, kernel);
	image_conv_kernel_f32_f32(dyy, &dyy_g, kernel);
	image_conv_kernel_f32_f32(dxy, &dxy_g, kernel);
//	write_image_data_to_file_float(str1,&dxx_g);
//	write_image_data_to_file_float(str2,&dxy_g);
//	write_image_data_to_file_float(str3,&dyy_g);
	det_of_mat(&dxx_g, &dyy_g, &dxy_g,point,thres);
	image_free(dxx);
	image_free(dyy);
	image_free(dxy);
}


void harris_corner(image_t *img,int8_t *kx,int8_t *ky,uint8_t size,kernel_t *kernel,feature_point_t *point,float thres)
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
	harris_corner_gauss_filter(&dxx,&dyy,&dxy,kernel,point,thres);
}

void test_harris_corner()
{	
	int m = 480;
	int n = 640;
	kernel_t kernel;
	feature_point_t point;
	uint8_t kernel_size = 7;
	float sigma = 1.0f;
	float thres = 0.01;
	image_t image, image1, image2;
	image_create(&image, m, n, IMAGE_GRAYSCALE);
	image_create(&image1, m - 1, n - 1, IMAGE_GRADIENT);
	image_create(&image2, m - 1, n - 1, IMAGE_GRADIENT);
	FILE *fp = fopen("data.dat", "r");
	if (fp == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		read_data_from_file_uint8(fp, image.buf, m, n);
	}
	fclose(fp);

	feature_point_create(&point, m, n);
	kernel_create(&kernel,kernel_size,KERNEL_F32);
	gauss_kernel_2d(&kernel, sigma, sigma);
	harris_corner(&image, fx, fy, 3,&kernel,&point,thres);
}