#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

#include "utils/types.h"
#include "utils/io.h"
#include "core/system/multitasking.h"
#include "hardware/interrupts.h"


class Syscall_handler: public Interrupt_handler {
private:

public:
	Syscall_handler(uint8_t interrupt_number);
	~Syscall_handler();

	virtual uint32_t handle(uint32_t esp) override;
};


#endif
