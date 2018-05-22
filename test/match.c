#include "file_wr.h"
#include "match.h"
#include "image.h"

uint32_t calc_image_teplate_diff_grayscale_l1(sad_t *sad,image_t *img,image_t *img_t,uint16_t x_s,uint16_t y_s)
{
	uint8_t *img_buf = (uint8_t *)img->buf;
	uint8_t *img_t_buf = (uint8_t *)img_t->buf;
	uint16_t i, j,idx,idy; 
	int tmp;
	uint32_t img_index,img_t_index, diff = 0;
	idx = x_s + img_t->w;
	idy = y_s + img_t->h;
	for (i = x_s; i < idx; i++)
	{
		for (j = y_s; j < idy; j++)
		{
			img_index = i * img->h + j;
			img_t_index = (i - x_s) * img_t->h + (j - y_s);
			tmp = img_buf[img_index] - img_t_buf[img_t_index];
			if (tmp < 0)
			{
				tmp = -tmp;
			}
			diff += tmp;
			if (diff > sad->sad_min_val )
			{
				break;
			}
		}
	}
	return diff;
}
void sad_matcher_init(sad_t *sad)
{
	sad->sad_min_val = 1e5;
}



void sad_matcher(sad_t *sad,image_t *img, image_t *img_t)
{
	uint8_t *img_buf = (uint8_t *)img->buf;
	uint8_t *img_t_buf = (uint8_t *)img_t->buf;
	uint32_t i, j,idx,idy;
	uint32_t sad_val = 0;
	idx = img->w - img_t->w;
	idy = img->h - img_t->h;
	for (i = 0; i < idx; i++)
	{
		for (j = 0; j < idy; j++)
		{
			sad_val = calc_image_teplate_diff_grayscale_l1(sad,img,img_t,i,j);
			if (sad_val < sad->sad_min_val)
			{
				sad->sad_min_val = sad_val;
				sad->best_x = i;
				sad->best_y = j;
				printf("%d %d %d \n", sad->best_x + 1, sad->best_y + 1, sad_val);
 			}
			sad_val = 0;
		}
	}

}
uint32_t ssad_calc_img_t_avg(image_t *img_t)
{
	uint32_t i,j,avg_img_t = 0;
	uint8_t *img_t_buf = (uint8_t *)img_t->buf;
	for (i = 0; i < img_t->w; i++)
	{
		for (j = 0; j < img_t->h; j++)
		{
			avg_img_t += img_t_buf[i*img_t->h + j];
		}
	}
	avg_img_t /= (uint32_t)((float)(img_t->w*img_t->h));
	return avg_img_t;
}

uint32_t ssad_calc_img_avg(image_t *img, int x, int y,int x_l,int y_l)
{
	int i, j, avg_img = 0;
	uint8_t *img_buf = (uint8_t *)img->buf;
	for (i = x; i < x + x_l; i++)
	{
		for (j = y; j < y + y_l; j++)
		{
			avg_img += img_buf[i*y_l + j];
		}
	}
	avg_img /= (uint32_t)((float)(x_l*y_l));
	return avg_img;
}

void ssad_calc_img_diff(image_t *dif,image_t *img_t,uint32_t avg_img)
{
	uint32_t i, j, index;
	int16_t *dif_buf = (int16_t *)dif->buf;
	uint8_t *img_t_buf = (uint8_t *)img_t->buf;
	for (i = 0; i < img_t->w; i++)
	{
		for (j = 0; j < img_t->h; j++)
		{
			index = i * img_t->h + j;
			dif_buf[index] = img_t_buf[index] - avg_img;
		}
	}
}
uint32_t ssad_calc_eps(image_t *img,image_t *img_t,int x,int y)
{
	uint32_t i, j,index,index_t,tmp,eps = 0;
	int16_t *img_buf = (int16_t *)img->buf;
	int16_t *img_t_buf = (int16_t *)img_t->buf;
	for (i = x; i < x+img->w;i++)
	{
		for (j = y; j < y+img->h; j++)
		{
			index = i * img->h + j;
			index_t = (i - x)*img_t->h + (j - y);
			tmp = img_buf[index] - img_t_buf[index_t];
			if (tmp < 0)
			{
				eps -= tmp;
			}
			eps += tmp;
		}
	}
	return eps;

}


void ssad_matcher_init(ssad_t *ssad,int thres)
{
	ssad->thres = thres;
}
void ssad(ssad_t *ssad,image_t *img,image_t *img_t)
{
	uint8_t *img_buf = (uint8_t *)img->buf;
	uint8_t *img_t_buf = (uint8_t *)img_t->buf;
	uint32_t avg_img_t = 0, avg_img = 0;
	uint32_t i, j, eps = 0, cnt = 0;
	image_t image, image_t;
	image_create(&image, img_t->w, img_t->h, IMAGE_GRAYSCALE);
	image_create(&image_t, img_t->w, img_t->h, IMAGE_GRAYSCALE);
	avg_img_t = ssad_calc_img_t_avg(img_t,0,0);
	ssad_calc_img_diff(&image_t,img_t,avg_img_t);
	for (i = 0; i < img->w - img_t->w; i++)
	{
		for (j = 0; j < img->h - img_t->h; j++)
		{
			eps = 0,cnt = 0;
			avg_img = ssad_calc_img_avg(img,i,j,img_t->w,img_t->h);
			ssad_calc_img_diff(&image,img,avg_img);
			if (eps < ssad->thres)
			{
				eps += ssad_calc_eps(&image, &image_t, i, j);
				cnt++;
			}
			if (cnt > ssad->match_num)
			{
				ssad->match_num = cnt;
				ssad->s_x = i;
				ssad->s_y = j;
			}
			if (ssad->match_num > ssad->max_match_num)
			{
				break;
			}
		}
	}
	image_free(&image);
	image_free(&image_t);
}
void test_sad()
{
	int m = 480;
	int n = 640;
	int m_t = 100;
	int n_t = 100;
	image_t image, image_t;
	image_create(&image, m, n, IMAGE_GRAYSCALE);
	image_create(&image_t, m_t, n_t, IMAGE_GRAYSCALE);
	FILE *fp = fopen("image.dat", "r+");
	FILE *fp_t = fopen("image_t.dat", "r+");
	if (fp == NULL || fp_t == NULL)
	{
		printf("NO file \n");
	}
	else
	{
		read_data_from_file_uint8(fp, image.buf, m, n);
		read_data_from_file_uint8(fp_t, image_t.buf, m_t, n_t);
	}
	sad_t sad;
	sad_matcher_init(&sad);
	sad_matcher(&sad,&image,&image_t);
	fclose(fp);
	fclose(fp_t);
}