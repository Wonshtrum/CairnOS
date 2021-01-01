#include "utils/types.h"
#include "utils/io.h"
#include "memory/memory.h"
#include "hardware/gdt.h"
#include "hardware/port.h"
#include "hardware/interrupts.h"
#include "hardware/pci.h"
#include "drivers/all.h"
#include "core/handlers/all.h"
#include "core/multitasking.h"

#define GRAPHICSMODE 1


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
	
	Print_keyboard keyboard_handler;
	Driver_keyboard keyboard(&keyboard_handler);
	driver_manager.add_driver(&keyboard);

	Text_mode_mouse mouse_handler(80, 25);
	Driver_mouse mouse(&mouse_handler);
	driver_manager.add_driver(&mouse);

	Driver_VGA vga;
	driver_manager.add_driver(&vga);

	print_str("\n");
	Peripheral_component_interconnect_controller PCI_controller;
	PCI_controller.select_drivers(2);
	print_str("\n");

/*
	// interrupt 14
	Driver_ATA ata0m(0x1F0, true);
	print_str("\nATA primary master: ");
	ata0m.identify();
	Driver_ATA ata0s(0x1F0, false);
	print_str("\nATA primary slave: ");
	ata0s.identify();
	print_str("\n");

	//char* ata_buffer_rec = "----------------------";
	//ata0m.read_28(0, (uint8_t*)ata_buffer_rec, 17);
	char* ata_buffer = "random text to test HD";
	ata0m.write_28(0, (uint8_t*)ata_buffer, 17);
	ata0m.flush();
	ata0m.read_28(0, (uint8_t*)ata_buffer, 17);
	print_str("\n");

	// interrupt 15
	Driver_ATA ata1m(0x170, true);
	Driver_ATA ata1s(0x170, false);
	// third: 0x1E8
	// fourth: 0x168
*/

	uint32_t g_modes[][3] = {
		{ 40, 25, 0 },
		{ 40, 50, 0 },
		{ 80, 25, 0 },
		{ 80, 50, 0 },
		{ 90, 30, 0 },
		{ 90, 60, 0 },
		{ 640, 480, 2 },
		{ 320, 200, 4 },
		{ 640, 480, 16 },
		{ 720, 480, 16 },
		{ 320, 200, 256 }
	};

	uint8_t mode = 2;
	vga.set_mode(g_modes[mode][0], g_modes[mode][1], g_modes[mode][2]);

	for (uint32_t x = 0 ; x < 320 ; x++) {
		for (uint32_t y = 0 ; y < 200 ; y++) {
			vga.put_pixel(x, y, 0x00, 0x00, 0xA8);
		}
	}

	driver_manager.activate_all();
	print_str("\n");

	interrupt_manager.activate();
	print_str("\nIDT activated\n");

	while (1);
}
