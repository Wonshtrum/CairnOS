#include "utils/types.h"
#include "utils/io.h"
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

void task_A() {
	while(1) print_str("A");
}
void task_B() {
	while(1) print_str("B");
}

extern "C" void kernel_main(void* multiboot_structure, uint32_t magic_number) {
	print_str("Hello world!\n", true);
	print_hex(magic_number);

	Global_descriptor_table gdt;
	print_str("GDT loaded\n");

	Task_manager task_manager;
	print_str("TM created\n");
	Task t_a(&gdt, task_A);
	Task t_b(&gdt, task_B);
	task_manager.add_task(&t_a);
	task_manager.add_task(&t_b);

	Interrupt_manager interrupt_manager(&gdt, &task_manager);
	print_str("IDT initialized\n");

	//hook hardware
	Keyboard_driver keyboard_driver;
	Mouse_driver mouse_driver;
	Peripheral_component_interconnect_controller PCI_controller;
	PCI_controller.select_drivers();
	Video_graphics_array vga;

#if GRAPHICSMODE
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
