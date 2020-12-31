#include "utils/types.h"
#include "utils/io.h"
#include "memory/memory.h"
#include "hardware/gdt.h"
#include "hardware/port.h"
#include "hardware/interrupts.h"
#include "hardware/pci.h"
#include "drivers/all.h"
#include "core/multitasking.h"

#define GRAPHICSMODE 0


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

	Global_descriptor_table gdt;
	print_str("GDT loaded\n");

	size_t heap = 10*1024;
	uint32_t* memory_upper = (uint32_t*)(((size_t)multiboot_structure) + 8);
	Memory_mamanger memory_mamanger(heap * 1024, ((*memory_upper) - heap - 10) * 1024);
	print_str("Memory mamanger created\n");

	Driver_manager driver_manager;
	print_str("Driver mamanger created\n");

	Task_manager task_manager;
	print_str("Task manager created\n");

	Interrupt_manager interrupt_manager(&gdt, &task_manager);
	print_str("IDT initialized\n");

	//hook hardware
	Keyboard_driver keyboard_driver;
	Mouse_driver mouse_driver;
	Peripheral_component_interconnect_controller PCI_controller;
	PCI_controller.select_drivers();

	driver_manager.activate_all();

#if GRAPHICSMODE
	Video_graphics_array vga;
	vga.set_mode(320, 200, 8);
	for (uint32_t x = 0 ; x < 320 ; x++) {
		for (uint32_t y = 0 ; y < 200 ; y++) {
			vga.put_pixel(x, y, 0x00, 0x00, 0xA8);
		}
	}
#endif

	interrupt_manager.activate();
	print_str("IDT activated\n");

	while (1);
}
