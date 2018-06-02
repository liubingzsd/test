#include "mat.h"
#include "mat_math.h"

mat_s *mat_create(uint16_t rows,uint16_t cols,enum mat_type type)
{
	mat_s *mat = NULL;
	mat = (mat_s *)malloc(sizeof(mat_s));
	mat->type = type;
	mat->rows = rows;
	mat->cols = cols;

	if (type == MAT_S16 || type == MAT_U16) 
	{
		mat->buf_size = sizeof(uint8_t) * 2 * rows * cols;
	}
	else if (type == MAT_U32 || type == MAT_S32) 
	{
		mat->buf_size = sizeof(uint8_t) * 4 * rows * cols;
	}
	else if (type == MAT_U16 || type == MAT_S16) 
	{
		mat->buf_size = sizeof(int16_t) * rows * cols;
	}
	else 
	{
		mat->buf_size = sizeof(uint8_t) * rows * cols;
	}
	mat->buf = malloc(mat->buf_size);
	if (mat->buf)
	{
	}
	else
	{
		printf("___func___ image_create \n");
		printf("mat buf allocated failed \n");
	}
	return mat;
}

void mat_free(mat_s *mat)
{
	if (mat->buf != NULL)
	{
		free(mat->buf);
		mat->buf = NULL;
	}
}

void mat_set_zero(mat_s *mat)
{
	uint16_t i, j;
	uint32_t idx, x_idx;
	uint8_t *mat_buf = NULL;
	if (mat->type == MAT_U8 || mat->type == MAT_S8)
	{
		uint8_t *mat_buf = (uint8_t *)mat->buf;
	}
	else if (mat->type == MAT_U16 || mat->type == MAT_S16)
	{
		uint16_t *mat_buf = (uint16_t *)mat->buf;
	}
	else if (mat->type == MAT_U32 || mat->type == MAT_S32)
	{
		uint32_t *mat_buf = (uint32_t *)mat->buf;
	}
	else
	{
		float *mat_buf = (float *)mat->buf;
	}
	for (i = 0; i < mat->rows; i++)
	{
		x_idx = i * mat->cols;
		for (j = 0; j < mat->cols; j++)
		{
			idx = x_idx + j;
			mat_buf[idx] = 0;
		}
	}
}


void mat_set_val_int(mat_s *mat,int val)
{
	uint16_t i, j;
	uint32_t idx, x_idx;
	if (mat->type == MAT_U8 || mat->type == MAT_S8)
	{
		uint8_t *mat_buf = (uint8_t *)mat->buf;
		for (i = 0; i < mat->rows; i++)
		{
			x_idx = i * mat->cols;
			for (j = 0; j < mat->cols; j++)
			{
				idx = x_idx + j;
				mat_buf[idx] = val;
			}
		}
	}
	else if (mat->type == MAT_U16 || mat->type == MAT_S16)
	{
		uint16_t *mat_buf = (uint16_t *)mat->buf;
		for (i = 0; i < mat->rows; i++)
		{
			x_idx = i * mat->cols;
			for (j = 0; j < mat->cols; j++)
			{
				idx = x_idx + j;
				mat_buf[idx] = val;
			}
		}
	}
	else 
	{
		uint32_t *mat_buf = (uint32_t *)mat->buf;
		for (i = 0; i < mat->rows; i++)
		{
			x_idx = i * mat->cols;
			for (j = 0; j < mat->cols; j++)
			{
				idx = x_idx + j;
				mat_buf[idx] = val;
			}
		}
	}
}

void mat_set_val_f32(mat_s *mat, float val)
{
	uint16_t i, j;
	uint32_t idx, x_idx;
	float *mat_buf = (float *)mat->buf;
	for (i = 0; i < mat->rows; i++)
	{
		x_idx = i * mat->cols;
		for (j = 0; j < mat->cols; j++)
		{
			idx = x_idx + j;
			mat_buf[idx] = val;
		}
	}
}

void test_mat()
{	
	int m = 10;
	int n = 10;
	mat_s *mat_u8 = NULL,*mat_u16 = NULL,*mat_u32 = NULL,*mat_f32 = NULL;

	mat_f32 = mat_create(m, n, MAT_F32);

	mat_set_val_f32(mat_f32, 7890.5643f);

	mat_printf_float(mat_f32->buf, m, n);

	mat_free(mat_f32);

}

