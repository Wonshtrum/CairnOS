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

	Task_manager task_manager;
	print_str("Task manager created\n");

	Interrupt_manager interrupt_manager(&gdt, &task_manager);
	print_str("IDT initialized\n");

	//hook hardware
	Keyboard_driver keyboard_driver;
	Mouse_driver mouse_driver;
	Peripheral_component_interconnect_controller PCI_controller;
	PCI_controller.select_drivers();
	Video_graphics_array vga;

	print_str("MEM: ");
	print_hex(*memory_upper);
	print_str("KiB\nHEAP: ");
	print_hex((*memory_upper) - heap - 10);
	print_str("KiB\n");

	print_str("\nstart: ");
	print_hex(heap * 1024);
	print_str("\n");
	void* alloc1 = memory_mamanger.malloc(1024);
	print_str("\nalloc1: ");
	print_hex((uint32_t)alloc1);
	print_str("\n");
	void* alloc2 = memory_mamanger.malloc(2048);
	print_str("\nalloc2: ");
	print_hex((uint32_t)alloc2);
	print_str("\n");
	void* alloc3 = memory_mamanger.malloc(8);
	print_str("\nalloc3: ");
	print_hex((uint32_t)alloc3);
	print_str("\n");
	print_str("free1\n");
	memory_mamanger.free(alloc3);
	print_str("free2\n");
	memory_mamanger.free(alloc1);
	print_str("free3\n");
	memory_mamanger.free(alloc2);

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
