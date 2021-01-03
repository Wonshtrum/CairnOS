#include "syscalls.h"


Syscall_handler::Syscall_handler(uint8_t interrupt_number): Interrupt_handler(interrupt_number + INTERRUPT_OFFSET) {}
Syscall_handler::~Syscall_handler() {}

uint32_t Syscall_handler::handle(uint32_t esp) {
	CPU_state* cpu = (CPU_state*)esp;
	switch (cpu->eax) {
		case 1:	// sys_exit
			break;
		case 2: // sys_fork
			break;
		case 3: // sys_read
			break;
		case 4: // sys_write
			print_str((char*)cpu->ebx);
			break;
		default:
			break;

	}
	return esp;
}
