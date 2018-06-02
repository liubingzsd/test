#include "image.h"
#include "kernel.h"
#include "image_filter.h"
#include "sift_test.h"

#define GAUSSKERN 3.5
#define SCALESPEROCTAVE 2
#define MAXOCTAVES      4
#define CURVATURE_THRESHOLD    10.0
#define DOUBLE_BASE_IMAGE_SIZE 1
#define CONTRAST_THRESHOLD     0.02  
#define max(a,b)(a>b?a:b)
#define min(a,b)(a<b?a:b)
image_octave_s *dog_octaves,*mag_pyr,*grad_pyr;
key_point_s *key_points = NULL;
key_point_s *key_descriptors = NULL;


/*
uint8_t image_blur_ker_size(float sigma)
{
	uint8_t size;
	size = size = (uint8_t)max(3.0f, 2.0f * GAUSSKERN * sigma + 1.0f);
	if (size % 2 == 0)
	{
		size += 1;
	}
	return size;
}

void image_blur_f32(image_s *src,image_s *dst,float sigma)
{
	kernel_s ker;
	uint8_t size;
	image_s dst1;
	image_create(&dst1, dst->rows, dst->cols, IMAGE_F32);
	size = image_blur_ker_size(sigma);
	kernel_create_1d(&ker,size,KERNEL_F32);
	gauss_kernel_1d(&ker,sigma);
	image_filter_1d_x_f32(src, &dst1, &ker, NONE);
	image_filter_1d_y_f32(&dst1, dst, &ker, NONE);
	image_free(&dst1);
}


image_s *image_scale_init(image_s *src)
{
	float init_sigma = 0.5f;
	float preblur_sigma = 1.0f;
	float sigma = 0.0f;
	image_s *img_tmp,*dst;
	img_tmp = image_create_ptr(src->rows,src->cols,IMAGE_F32);
	image_blur_f32(src,img_tmp,init_sigma);
	if (DOUBLE_BASE_IMAGE_SIZE)
	{
		uint32_t rows, cols;
		rows = 2 * src->rows;
		cols = 2 * src->cols;
		image_s *img_double,*img_double_blur,*dst;
		img_double = image_create_ptr(rows, cols, IMAGE_F32);
		img_double_blur = image_create_ptr(rows, cols, IMAGE_F32);
		dst = image_create_ptr(rows, cols, IMAGE_F32);
		image_resize_double_f32(img_tmp,img_double);
		image_blur_f32(img_double,img_double_blur,preblur_sigma);
		sigma = sqrt((4 * init_sigma * init_sigma) + preblur_sigma * preblur_sigma);
		image_blur_f32(img_double_blur,dst,sigma);
		image_free(img_double);
		image_free(img_double_blur);
	}
	else
	{
		image_s *dst;
		dst = image_create_ptr(src->rows, src->cols, IMAGE_F32);
		sigma = sqrt((4 * init_sigma * init_sigma) + preblur_sigma * preblur_sigma);
		image_blur_f32(img_tmp,dst,sigma);
	}
	image_free(img_tmp);
}

image_octave_s *build_gaussian_octaves(image_s *src)
{
	image_octave_s *octaves;
	image_s *img_tmp,*dst,*temp;
	int i, j;
	float k;
	float initial_sigma, sigma, absolue_sigma, sigma_f;
	k = pow(2, 1.0f / (float)(SCALESPEROCTAVE));
	int dim = min(src->rows, src->cols);
	int num_octaves = (int)(log(dim) / log(2.0)) - 2;
	num_octaves = min(num_octaves, MAXOCTAVES);
	octaves = (image_octave_s *)malloc(num_octaves * sizeof(image_octave_s));
	dog_octaves = (image_octave_s *)malloc(num_octaves * sizeof(image_octave_s));
	img_tmp = image_clone_f32(src);
	initial_sigma = sqrt(2);
	for (i = 0; i < num_octaves; i++)
	{
		octaves[i].octave = (image_level_s *)malloc((SCALESPEROCTAVE + 3) * sizeof(image_level_s));
		dog_octaves[i].octave = (image_level_s *)malloc((SCALESPEROCTAVE + 2) * sizeof(image_level_s));
		(octaves[i].octave)[0].level = img_tmp;
		octaves[i].cols = img_tmp->cols;
		octaves[i].rows = img_tmp->rows;
		dog_octaves[i].cols = img_tmp->cols;
		dog_octaves[i].rows = img_tmp->rows;
		if (DOUBLE_BASE_IMAGE_SIZE)
		{
			octaves->sub_sample = pow(2, i)*0.5f;
		}
		else
		{
			octaves->sub_sample = pow(2, i);
		}
		if (i == 0)
		{
			(octaves[0].octave)[0].level_sigma = initial_sigma;
			(octaves[0].octave)[0].absolute_sigma = initial_sigma;
		}
		else
		{
			(octaves[i].octave)[0].level_sigma = (octaves[i - 1].octave)[SCALESPEROCTAVE].level_sigma;
			(octaves[i].octave)[0].absolute_sigma = (octaves[i - 1].octave)[SCALESPEROCTAVE].absolute_sigma;
		}
		for (j = 1; j < SCALESPEROCTAVE; j++)
		{
			dst = image_create_ptr(img_tmp->rows, img_tmp->cols, IMAGE_F32);
			temp = image_create_ptr(img_tmp->rows, img_tmp->cols, IMAGE_F32);
			sigma_f = sqrt(k * k - 1)*sigma;
			sigma = k * sigma;
			absolue_sigma = sigma * (octaves[i].sub_sample);

			(octaves[i].octave)[j].level_sigma = sigma;
			(octaves[i].octave)[j].absolute_sigma = absolue_sigma;
			int length = image_blur_ker_size(sigma_f);
			image_blur_f32((octaves[i].octave)[j-1].level,dst,sigma_f);
			(octaves[i].octave)[j].level_sigma_length = length;
			(octaves[i].octave)[j].level = dst;
			image_sub((octaves[i].octave)[j].level, (octaves[i].octave)[j-1].level,temp);
			(dog_octaves[i].octave)[j - 1].level = temp;
		}
		img_tmp = image_resize_half_f32((octaves[i].octave)[SCALESPEROCTAVE].level);	
	}
	return octaves;
}

float img_level(uint8_t i,uint8_t j,uint16_t m,uint16_t n)
{
	float tmp;
	float *buf = (float *)(dog_octaves[i]).octave[j].level->buf;
	uint16_t col = (dog_octaves[i]).octave[j].level->cols;
	tmp = buf[m * col + n];
	return tmp;
}


int detect_key_point(int num_octaves,image_octave_s *gaussian_pyr)
{
	float curvature_threshold;
	curvature_threshold = ((CURVATURE_THRESHOLD + 1)*(CURVATURE_THRESHOLD + 1)) / CURVATURE_THRESHOLD;
	int key_point_count = 0;
	for (int i = 0; i < num_octaves; i++)
	{
		for (int j = 1; j < SCALESPEROCTAVE + 1; j++)
		{
			int dim = (int)(0.5f * (gaussian_pyr[i].octave)[j].level_sigma_length + 0.5f);
			for (int m = dim; m < dog_octaves[i].rows - dim; m++)
			{
				for (int n = dim; n < dog_octaves[i].cols - dim; n++)
				{
					if (fabs(img_level(i, j, m, n)) >= CONTRAST_THRESHOLD)
					{
						if (img_level(i, j, m, n) != 0.0f)
						{
							float inf_val = img_level(i, j, m, n);
							if (((inf_val <= img_level(i, j - 1, m - 1, n - 1)) &&
								(inf_val <= img_level(i, j - 1, m, n - 1)) &&
								(inf_val <= img_level(i, j - 1, m + 1, n - 1)) &&
								(inf_val <= img_level(i, j - 1, m - 1, n)) &&
								(inf_val <= img_level(i, j - 1, m, n)) &&
								(inf_val <= img_level(i, j - 1, m + 1, n)) &&
								(inf_val <= img_level(i, j - 1, m - 1, n + 1)) &&
								(inf_val <= img_level(i, j - 1, m, n + 1)) &&
								(inf_val <= img_level(i, j - 1, m + 1, n + 1)) &&    //底层的小尺度9

								(inf_val <= img_level(i, j, m - 1, n - 1)) &&
								(inf_val <= img_level(i, j, m, n - 1)) &&
								(inf_val <= img_level(i, j, m + 1, n - 1)) &&
								(inf_val <= img_level(i, j, m - 1, n)) &&
								(inf_val <= img_level(i, j, m + 1, n)) &&
								(inf_val <= img_level(i, j, m - 1, n + 1)) &&
								(inf_val <= img_level(i, j, m, n + 1)) &&
								(inf_val <= img_level(i, j, m + 1, n + 1)) &&     //当前层8

								(inf_val <= img_level(i, j + 1, m - 1, n - 1)) &&
								(inf_val <= img_level(i, j + 1, m, n - 1)) &&
								(inf_val <= img_level(i, j + 1, m + 1, n - 1)) &&
								(inf_val <= img_level(i, j + 1, m - 1, n)) &&
								(inf_val <= img_level(i, j + 1, m, n)) &&
								(inf_val <= img_level(i, j + 1, m + 1, n)) &&
								(inf_val <= img_level(i, j + 1, m - 1, n + 1)) &&
								(inf_val <= img_level(i, j + 1, m, n + 1)) &&
								(inf_val <= img_level(i, j + 1, m + 1, n + 1))     //下一层大尺度9        
								) ||
								((inf_val >= img_level(i, j - 1, m - 1, n - 1)) &&
								(inf_val >= img_level(i, j - 1, m, n - 1)) &&
									(inf_val >= img_level(i, j - 1, m + 1, n - 1)) &&
									(inf_val >= img_level(i, j - 1, m - 1, n)) &&
									(inf_val >= img_level(i, j - 1, m, n)) &&
									(inf_val >= img_level(i, j - 1, m + 1, n)) &&
									(inf_val >= img_level(i, j - 1, m - 1, n + 1)) &&
									(inf_val >= img_level(i, j - 1, m, n + 1)) &&
									(inf_val >= img_level(i, j - 1, m + 1, n + 1)) &&

									(inf_val >= img_level(i, j, m - 1, n - 1)) &&
									(inf_val >= img_level(i, j, m, n - 1)) &&
									(inf_val >= img_level(i, j, m + 1, n - 1)) &&
									(inf_val >= img_level(i, j, m - 1, n)) &&
									(inf_val >= img_level(i, j, m + 1, n)) &&
									(inf_val >= img_level(i, j, m - 1, n + 1)) &&
									(inf_val >= img_level(i, j, m, n + 1)) &&
									(inf_val >= img_level(i, j, m + 1, n + 1)) &&

									(inf_val >= img_level(i, j + 1, m - 1, n - 1)) &&
									(inf_val >= img_level(i, j + 1, m, n - 1)) &&
									(inf_val >= img_level(i, j + 1, m + 1, n - 1)) &&
									(inf_val >= img_level(i, j + 1, m - 1, n)) &&
									(inf_val >= img_level(i, j + 1, m, n)) &&
									(inf_val >= img_level(i, j + 1, m + 1, n)) &&
									(inf_val >= img_level(i, j + 1, m - 1, n + 1)) &&
									(inf_val >= img_level(i, j + 1, m, n + 1)) &&
									(inf_val >= img_level(i, j + 1, m + 1, n + 1))
									))      //2、满足26个中极值点
							{
								if (fabs(img_level(i, j, m, n)) >= CONTRAST_THRESHOLD)
								{
									float dxx, dyy, dxy, tr_h, det_h, curvature_ratio;
									dxx = img_level(i, j, m, n - 1) + img_level(i, j, m, n + 1) - 2.0f * img_level(i, j, m, n);
									dyy = img_level(i, j, m - 1, n) + img_level(i, j, m + 1, n) - 2.0f * img_level(i, j, m, n);
									dxy = img_level(i, j, m - 1, n - 1) + img_level(i, j, m + 1, n + 1) - img_level(i, j, m + 1, n - 1) - img_level(i, j, m - 1, n + 1);
									tr_h = dxx + dyy;
									det_h = dxx * dyy - dxy * dxy;
									curvature_ratio = (1.0f * tr_h * tr_h) / det_h;
									if (det_h > 0.0f && curvature_ratio <= curvature_threshold)
									{
										key_point_count++;
										key_point_s *k;
										k = (key_point_s *)malloc(sizeof(key_point_s));
										k->next = key_points;
										k->row = m * gaussian_pyr[i].sub_sample;
										k->row = n * gaussian_pyr[i].sub_sample;
										k->sy = m;
										k->sx = n;
										k->octave = i;
										k->level = j;
										k->scale = (gaussian_pyr[i].octave)[j].absolute_sigma;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return key_point_count;
}
void calc_grad_direc_mag(int num_octaves,image_octave_s *gaussian_pyr)
{
	mag_pyr = (image_octave_s *)malloc(num_octaves * sizeof(image_octave_s));
	grad_pyr = (image_octave_s *)malloc(num_octaves * sizeof(image_octave_s));
	for (int i = 0; i < num_octaves; i++)
	{
		mag_pyr[i].octave = (image_level_s *)malloc(SCALESPEROCTAVE * sizeof(image_level_s));
		grad_pyr[i].octave = (image_level_s *)malloc(SCALESPEROCTAVE * sizeof(image_level_s));
		for (int j = 1; j < SCALESPEROCTAVE + 1; j++)
		{
			image_s *mag = image_create_ptr(gaussian_pyr[i].rows, gaussian_pyr[i].cols, IMAGE_F32);
			image_s *ori = image_create_ptr(gaussian_pyr[i].rows, gaussian_pyr[i].cols, IMAGE_F32);
			image_s *temp_mat1 = image_create_ptr(gaussian_pyr[i].rows, gaussian_pyr[i].cols, IMAGE_F32);
			image_s *temp_mat2 = image_create_ptr(gaussian_pyr[i].rows, gaussian_pyr[i].cols, IMAGE_F32);
		}
	}
}




void test_sift()
{
	int m = 480;
	int n = 640;
	int num_octaves = 0;
	image_octave_s *gaussian_pyr;
	image_s *img = NULL;
	image_s *img_tmp = NULL;
	img = image_create_ptr(m,n,IMAGE_F32);
	image_convert_f32_scale(img,img,1.0f/255);
	int dim = min(img->rows,img->cols);
	num_octaves = (int)(log((float)dim) / log(2.0f)) - 2;
	num_octaves = min(num_octaves, MAXOCTAVES);
	img_tmp = image_scale_init(img);
	gaussian_pyr = build_gaussian_octaves(img_tmp);
	int key_point_num = detect_key_point(num_octaves,gaussian_pyr);

}

*/