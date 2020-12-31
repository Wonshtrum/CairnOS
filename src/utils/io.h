#ifndef __IO_H__
#define __IO_H__

#include "utils/types.h"


void print_str(char* str, bool clear = false);
void print_chr(char chr);

void print_hex(uint8_t  n);
void print_hex(uint16_t n);
void print_hex(uint32_t n);
void print_hex(uint64_t n);

void print_bin(uint8_t  n);
void print_bin(uint16_t n);
void print_bin(uint32_t n);
void print_bin(uint64_t n);

#endif
