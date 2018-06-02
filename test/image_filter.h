#ifndef _IMAGE_FILTER_H_
#define _IMAGE_FILTER_H_

enum boundary_type {
	NONE,
	ZERO,
	REPLICATE
};

void image_filter_1d_x_f32(image_s *src, image_s *dst, kernel_s *kernel, enum boundary_type type);
void image_filter_1d_y_f32(image_s *src, image_s *dst, kernel_s *kernel, enum boundary_type type);

void image_filter_2d_f32_f32(image_s *src, image_s *dst, kernel_s *kernel, enum boundary_type type);
void image_filter_1d_f32_f32(image_s *src, image_s *dst, kernel_s *kernel, enum boundary_type type);
void test_image_filter();
#endif
