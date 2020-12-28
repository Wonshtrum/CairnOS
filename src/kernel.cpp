#include "types.h"
#include "io.h"
#include "gdt.h"
#include "port.h"
#include "interrupts.h"
#include "drivers/drivers.h"


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors() {
	for (constructor* i = &start_ctors ; i != &end_ctors ; i++) {
		(*i)();
	}
}

extern "C" void kernel_main(void* multiboot_structure, uint32_t magic_number) {
	print_str("Hello world!\n", true);
	print_hex(magic_number);
	print_str("\n");


	Global_descriptor_table gdt;
	print_str("GDT loaded\n");

	Interrupt_manager interrupt_manager(&gdt);
	print_str("IDT initialized!\n");

	//hook hardware
	Keyboard_driver keyboard_driver;

	interrupt_manager.activate();
	print_str("IDT activated\n");

	while (1);
}
