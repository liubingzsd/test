#ifndef _MAT_H_
#define _MAT_H_
#include <stdint.h>

enum mat_type {
	MAT_F32,     
	MAT_U8,
	MAT_S8,
	MAT_U16,
	MAT_S16,
	MAT_U32,
	MAT_S32
};

typedef struct
{
	enum mat_type type;
	uint16_t rows;
	uint16_t cols;
	uint32_t buf_size;
	void *buf;
}mat_s;



#endif
