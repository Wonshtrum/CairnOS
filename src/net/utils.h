#ifndef __NET_UTILS_H__
#define __NET_UTILS_H__

#include "utils/types.h"
#include "utils/io.h"


void print_ip4(uint32_t ip);

uint32_t broadcast_ip(uint32_t ip, uint32_t mask);


#endif
