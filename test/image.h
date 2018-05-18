#ifndef _IMAGE_H_
#define _IMAGE_H_
#include <stdio.h>
#include <stdint.h>

enum image_type {
	IMAGE_YUV422,     ///< UYVY format (uint16 per pixel)
	IMAGE_GRAYSCALE,  ///< Grayscale image with only the Y part (uint8 per pixel)
	IMAGE_JPEG,       ///< An JPEG encoded image (not per pixel encoded)
	IMAGE_GRADIENT    ///< An image gradient (int16 per pixel)
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
	uint16_t w;
	uint16_t h;
	uint32_t buf_size;
	void *buf;
}image_t;

void test_image();
void image_free(image_t *img);
void image_create(image_t *img, uint16_t width, uint16_t height, enum image_type type);
void write_image_data_to_file_uint8(char *str, image_t *img);
void write_image_data_to_file_int16(char *str, image_t *img);

#endif
