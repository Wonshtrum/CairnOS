#include "io.h"


void print_str(char* str, bool clear) {
	static uint16_t* video_mem = (uint16_t*)0xb8000;
	static uint8_t x = 0, y = 0;
	if (clear) {
		for (y = 0 ; y < 25 ; y++) {
			for (x = 0 ; x < 80 ; x++) {
				video_mem[80*y+x] &= 0xFF00;
			}
		}
		x = 0;
		y = 0;
	}
	for (int i = 0 ; str[i] != '\0' ; i++) {
		switch (str[i]) {
			case '\n':
				y++;
				x = 0;
				break;
			default:
				video_mem[80*y+x] = (video_mem[i] & 0xFF00) | str[i];
				x++;
				break;
		}
		if (x >= 80) {
			y++;
			x = 0;
		}
		if (y >= 25) {
			for (y = 0 ; y < 25 ; y++) {
				for (x = 0 ; x < 80 ; x++) {
					if (y < 24) {
						video_mem[80*y+x] = video_mem[80*(y+1)+x];
					} else {
						video_mem[80*y+x] &= 0xFF00;
					}
				}
			}
			x = 0;
			y = 24;
		}
	}
}

static char* chr_template = "*";
void print_chr(char chr) {
	chr_template[0] = chr;
	print_str(chr_template);
}

static char* hex_table = "0123456789abcdef";
static char* hex_template = "0x****************G";
#define _PRINT_HEX(T, N) void print_hex(T n) {\
	for (int i = 0 ; i < N/4 ; i++) {\
		hex_template[N/4+1-i] = hex_table[(n >> (i*4)) & 0xF];\
	}\
	hex_template[N/4+2] = '\0';\
	print_str(hex_template);\
}
#define PRINT_HEX(N) _PRINT_HEX(int##N##_t, N) _PRINT_HEX(uint##N##_t, N)
PRINT_HEX( 8);
PRINT_HEX(16);
PRINT_HEX(32);
PRINT_HEX(64);

static char* bin_template = "0b****************************************************************G";
#define _PRINT_BIN(T, N) void print_bin(T n) {\
	for (int i = 0 ; i < N ; i++) {\
		bin_template[N+1-i] = ((n >> i) & 1) ? '1' : '0';\
	}\
	bin_template[N+2] = '\0';\
	print_str(bin_template);\
}
#define PRINT_BIN(N) _PRINT_BIN(int##N##_t, N) _PRINT_BIN(uint##N##_t, N)
PRINT_BIN( 8);
PRINT_BIN(16);
PRINT_BIN(32);
PRINT_BIN(64);
