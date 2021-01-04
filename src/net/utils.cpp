#include "utils.h"


void print_ip4(uint32_t ip) {
	print_hex((uint8_t)((ip >>  0) & 0xFF), false); print_str(".");
	print_hex((uint8_t)((ip >>  8) & 0xFF), false); print_str(".");
	print_hex((uint8_t)((ip >> 16) & 0xFF), false); print_str(".");
	print_hex((uint8_t)((ip >> 24) & 0xFF), false);
}

uint32_t broadcast_ip(uint32_t ip, uint32_t mask) {
	return ip | ~mask;
}
