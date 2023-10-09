#ifndef INCLUDE_TYPE_H
#define INCLUDE_TYPE_H

#include "common.h"

typedef unsigned char sb_byte;

typedef struct {
	int length;
	sb_byte* bytes;
	int index;
} sb_buffer;

#endif
