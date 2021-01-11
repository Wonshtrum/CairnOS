#include "utils/types.h"
#include "utils/bits.h"
#include "utils/io.h"
#include "memory/memory.h"
#include "hardware/gdt.h"
#include "hardware/port.h"
#include "hardware/interrupts.h"
#include "hardware/pci.h"
#include "drivers/all.h"
#include "core/handlers/all.h"
#include "core/system/multitasking.h"
#include "core/system/syscalls.h"
#include "core/system/hooks/syscalls.h"
#include "net/all.h"
#include "gui/all.h"

#define GRAPHICSMODE 10


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors() {
	for (constructor* i = &start_ctors ; i != &end_ctors ; i++) {
		(*i)();
	}
}


void taskA() {
	while (true) {
		sys_print_str("A");
	}
}
void taskB() {
	while (true) {
		sys_print_str("B");
	}
}


extern "C" void kernel_main(void* multiboot_structure, uint32_t magic_number) {
	print_str("Hello world!\n", true);
	print_hex(magic_number);


	/////////////////
	// GLOBAL CONF
	/////////////////
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
	Syscall_handler syscalls(0x80);
	print_str("Syscalls handler created\n");


	/////////////////
	// DRIVERS
	/////////////////
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
	PCI_controller.select_drivers(1);
	print_str("\n");

/*
	/////////////////
	// HARDDRIVE
	/////////////////
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

	/////////////////
	// SCREEN
	/////////////////
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
	uint8_t mode = GRAPHICSMODE;
	vga.set_mode(g_modes[mode][0], g_modes[mode][1], g_modes[mode][2]);
	#if GRAPHICSMODE >= 10
		Graphics_context* ctx = vga.get_graphics_context();
		Font font2x2 = Fonts::get(2, 2);
		Font font4x4 = Fonts::get(4, 4);
		Font font3x5 = Fonts::get(3, 5);
		Font font5x5 = Fonts::get(5, 5);
		Desktop desktop(ctx->get_width(), ctx->get_height(), {0x00, 0x00, 0xA8});
		Window w1(5, 5, 100, 50, {0, 0, 0});
		Window w2(70, 40, 120, 80, {0, 0xA8, 0});
		Widget f1(5, 5, 30, 30, {0xFF, 0xFF, 0xFF});

		Text t1(2,  2, 100, 2, {0, 0, 0}, font2x2);
		Text t2(2,  6, 100, 4, {0, 0, 0}, font4x4);
		Text t3(2, 12, 100, 5, {0, 0, 0}, font3x5);
		Text t4(2, 19, 100, 5, {0, 0, 0}, font5x5);
		t1.add_text("Hello world");
		t2.add_text("Hello world");
		t3.add_text("Hello world");
		t4.add_text("Hello world");

		desktop.add_child(&w2);
		desktop.add_child(&w1);
		w2.add_child(&f1);
		w1.add_child(&f1);
		w2.add_child(&t1);
		w2.add_child(&t2);
		w2.add_child(&t3);
		w2.add_child(&t4);
		mouse.set_event_handler(&desktop);

		Bounding_box screen = desktop.get_bounding_box();
		desktop.draw(ctx, screen);
		desktop.set_ctx(ctx);
	#endif


	driver_manager.activate_all();
	print_str("\n");


	/////////////////
	// NETWORKING
	/////////////////
	uint32_t my_ip = make_ip_be(10, 0, 2, 15);
	uint32_t gw_ip = make_ip_be(10, 0, 2, 2);
	uint32_t mask = make_ip_be(255, 255, 255, 0);
	Driver_am79c973* eth0 = (Driver_am79c973*)driver_manager.debug_get(3);
	eth0->set_ip(my_ip);
	// ethernet
	Ethernet_frame_provider ethernet(eth0);
	// ethernet->arp
	Address_resolution_protocol arp(&ethernet);
	// ethernet->ipv4
	Internet_protocol_provider ipv4(&ethernet, &arp, gw_ip, mask);
	// ethernet->ipv4->icmp
	Internet_control_message_protocol icmp(&ipv4);
	// ethernet->ipv4->udp
	User_datagram_protocol_provider udp(&ipv4);



	interrupt_manager.activate();
	print_str("IDT activated\n\n");


/*
	uint64_t gw_mac = arp.resolve(gw_ip);
	gw_mac = arp.resolve(gw_ip);
	print_hex(gw_mac);
*/
	arp.broadcast_mac(mask);
	icmp.echo_reply(gw_ip);
	#if 0
	UDP_socket* socket = udp.connect(gw_ip, 1234);
	socket->send((uint8_t*)"Hello UDP!", 10);
	#else
	UDP_socket* socket = udp.listen(1234);
	Console_UDP udp_handler;
	socket->bind(&udp_handler);
	keyboard.set_event_handler(&udp_handler);
	#endif

/*
	/////////////////
	// MULTITASKING
	/////////////////
	Task ta = Task(&gdt, taskA);
	Task tb = Task(&gdt, taskB);
	task_manager.add_task(&ta);
	task_manager.add_task(&tb);
*/

/*
	print_str("\nHEAP STATUS:\n");
	memory_mamanger.diagnostic();
*/

	print_str("OK\n");
	while (true) {
	#if GRAPHICSMODE >= 10
		//desktop.draw(ctx, screen);
	#endif
	}
}
