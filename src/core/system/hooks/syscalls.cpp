#include "syscalls.h"


void sys_print_str(char* str) {
	asm("int $0x80" : : "a" (4), "b" (str));
}
