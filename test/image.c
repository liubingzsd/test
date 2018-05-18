#include <malloc.h>
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
	else {
		img->buf_size = sizeof(uint8_t) * width * height;
	}
	img->buf = malloc(img->buf_size);
	if (img->buf)
	{
		printf("image buf allocated at 0x%x \n", (unsigned int)img->buf);
	}
	else
	{
		printf("image buf allocated failed \n");
	}
}

void image_free(image_t *img)
{
	if (img->buf != NULL) {
		free(img->buf);
		img->buf = NULL;
	}
	else
	{
		img->buf = NULL;
	}
}

void calc_image_gradients(image_t *input, image_t *dx, image_t *dy)
{
	uint16_t i, j;
	uint8_t *input_buf = (uint8_t *)input->buf;
	int16_t *dx_buf = (int16_t *)dx->buf;
	int16_t *dy_buf = (int16_t *)dy->buf;
/*
	for (x = 1; x < input->w - 1; x++) 
	{
		for (y = 1; y < input->h - 1; y++) 
		{
			dx_buf[(y - 1)*dx->w + (x - 1)] = (int16_t)input_buf[y * input->w + x + 1] - (int16_t)input_buf[y * input->w + x - 1];
			dy_buf[(y - 1)*dy->w + (x - 1)] = (int16_t)input_buf[(y + 1) * input->w + x] - (int16_t)input_buf[(y - 1) * input->w + x];
		}
	}

	for (i = 0; i < input->w; i++)
	{
		for (j = 0; j < input->h; j++)
		{
			dx_buf[i*dx->h + j] = input_buf[(i + 1)*input->h + j] - input_buf[i*input->h + j];
			dy_buf[i*dy->h + j] = input_buf[i*input->h + j + 1] - input_buf[i*input->h + j];
		}

	}
*/
	for (i = 0; i < dx->w; i++)
	{
		for (j = 0; j < dx->h; j++)
		{
			dx_buf[i*dx->h + j] = 100;
//			dy_buf[i*dy->h + j] = 255;
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
	calc_image_gradients(&image, &image1, &image2);
	write_image_data_to_file_uint8(str, &image);
	write_image_data_to_file_int16(str1, &image1);
	write_image_data_to_file_int16(str2, &image2);
}
