#ifndef __BITS_H__
#define __BITS_H__

#include "utils/types.h"
#include "utils/io.h"

uint16_t reverse_endian(uint16_t x);
uint32_t reverse_endian(uint32_t x);
uint64_t reverse_endian(uint64_t x);

int16_t reverse_endian(int16_t x);
int32_t reverse_endian(int32_t x);
int64_t reverse_endian(int64_t x);

#endif
