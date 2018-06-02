#ifndef _IMAGE_H_
#define _IMAGE_H_
#include <stdio.h>
#include <stdint.h>
#include "kernel.h"

enum image_type {
	IMAGE_YUV422,     ///< UYVY format (uint16 per pixel)
	IMAGE_GRAYSCALE,  ///< Grayscale image with only the Y part (uint8 per pixel)
	IMAGE_JPEG,       ///< An JPEG encoded image (not per pixel encoded)
	IMAGE_GRADIENT,    ///< An image gradient (int16 per pixel)
	IMAGE_F32
};

typedef struct
{
	uint16_t x;
	uint16_t y;
}point_t;

typedef struct {
	uint16_t x;    ///< Start position x (horizontal)
	uint16_t y;    ///< Start position y (vertical)
	uint16_t w;    ///< Width of the cropped area
	uint16_t h;    ///< height of the cropped area
}crop_t;

typedef struct
{
	enum image_type type;
	uint16_t rows;
	uint16_t cols;
	uint32_t buf_size;
	void *buf;
}image_s;

void test_image();
void image_free(image_s *img);
image_s *image_clone(image_s *src);

image_s *image_clone_f32(image_s *img);
image_s *image_clone_uint8(image_s *img);
image_s *image_resize_half_f32(image_s *src);
void image_resize_double_f32(image_s *src, image_s *dst);
image_s *image_create_ptr(uint16_t rows, uint16_t cols, enum image_type type);
void image_convert_f32_scale(image_s *src, image_s *dst, float scale);
void image_convert_u8_f32(image_s *src, image_s *dst);

void image_sub(image_s *img1, image_s *img2, image_s *img3);
void image_create(image_s *img, uint16_t rows, uint16_t cols, enum image_type type);
void write_image_data_to_file_uint8(char *str, image_s *img);
void write_image_data_to_file_int16(char *str, image_s *img);
void write_image_data_to_file_float(char *str, image_s *img);
void image_conv_uint8_int16(image_s *img, image_s *img1, int8_t *ker, uint8_t ker_size);
void image_conv_int16_f32(image_s *img, image_s *img1, float *ker, uint8_t ker_size);
void image_conv_kernel_f32_f32(image_s *img, image_s *img1, kernel_s *kernel);
void image_conv_kernel_int16_f32(image_s *img, image_s *img1, kernel_s *kernel);

#endif
