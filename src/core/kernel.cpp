#include "utils/types.h"
#include "utils/io.h"
#include "hardware/gdt.h"
#include "hardware/port.h"
#include "hardware/interrupts.h"
#include "hardware/pci.h"
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
	Mouse_driver mouse_driver;
	Peripheral_component_interconnect_controller PCI_controller;
	PCI_controller.select_drivers();

	interrupt_manager.activate();
	print_str("IDT activated\n");

	while (1);
}
