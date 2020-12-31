#include "am79c973.h"


Driver_am79c973::Driver_am79c973(Device_descriptor* dev):
	Driver(),
	Interrupt_handler(dev->interrupt + INTERRUPT_OFFSET),
	mac_addr0_port(dev->port_base + 0x0),
	mac_addr2_port(dev->port_base + 0x2),
	mac_addr4_port(dev->port_base + 0x4),
	register_data_port(dev->port_base + 0x10),
	register_addr_port(dev->port_base + 0x12),
	reset_port(dev->port_base + 0x14),
	bus_controller_register_data_port(dev->port_base + 0x16) {
	
	current_send_buffer = 0;
	current_recv_buffer = 0;

	uint64_t mac_0 = mac_addr0_port.read() & 0xFF;
	uint64_t mac_1 = mac_addr0_port.read() >> 8;
	uint64_t mac_2 = mac_addr0_port.read() & 0xFF;
	uint64_t mac_3 = mac_addr0_port.read() >> 8;
	uint64_t mac_4 = mac_addr0_port.read() & 0xFF;
	uint64_t mac_5 = mac_addr0_port.read() >> 8;
	uint64_t mac = 0
		| mac_0 <<  0
		| mac_1 <<  8
		| mac_2 << 16
		| mac_3 << 24
		| mac_4 << 32
		| mac_5 << 40;

	// 32 bit mode
	register_addr_port.write(20);
	bus_controller_register_data_port.write(0x102);

	// STOP reset
	register_addr_port.write(0);
	register_data_port.write(0x04);

	// init block
	init.mode = 0x0000; // promiscuous mode = false
	init.reserved_1 = 0;
	init.num_send_buffers = 3;
	init.reserved_2 = 0;
	init.num_recv_buffers = 3;
	init.physical_addr = mac;
	init.reserved_3 = 0;
	init.logical_addr = 0;

	send_buffer_desc = (Buffer_descriptor*)((((uint32_t)send_buffer_desc_mem) + 15) & ~((uint32_t)0xF));
	init.send_buffer_desc_addr = (uint32_t)send_buffer_desc;

	recv_buffer_desc = (Buffer_descriptor*)((((uint32_t)recv_buffer_desc_mem) + 15) & ~((uint32_t)0xF));
	init.recv_buffer_desc_addr = (uint32_t)recv_buffer_desc;

	for (uint8_t i = 0 ; i < 8 ; i++) {
		send_buffer_desc[i].address = (((uint32_t)&send_buffers[i]) + 15) & ~((uint32_t)0xF);
		send_buffer_desc[i].flags_1 = 0xF7FF;
		send_buffer_desc[i].flags_2 = 0;
		send_buffer_desc[i].avail = 0;

		recv_buffer_desc[i].address = (((uint32_t)&recv_buffers[i]) + 15) & ~((uint32_t)0xF);
		recv_buffer_desc[i].flags_1 = 0x8000F7FF;
		recv_buffer_desc[i].flags_2 = 0;
		recv_buffer_desc[i].avail = 0;
	}

	register_addr_port.write(1);
	register_data_port.write((uint32_t)(&init) & 0xFFFF);
	register_addr_port.write(2);
	register_data_port.write(((uint32_t)(&init) >> 16) & 0xFFFF);
}

Driver_am79c973::~Driver_am79c973() {}

void Driver_am79c973::activate() {
	register_addr_port.write(0);
	register_data_port.write(0x41);

	register_addr_port.write(4);
	uint32_t temp = register_data_port.read();
	register_addr_port.write(4);
	register_data_port.write(temp | 0xC00);

	register_addr_port.write(0);
	register_data_port.write(0x42);
}

uint32_t Driver_am79c973::reset() {
	reset_port.read();
	reset_port.write(0);
	return 10;
}

uint32_t Driver_am79c973::handle(uint32_t esp) {
	print_str("INTERRUPT FROM AMD am79c973\n");
	register_addr_port.write(0);
	uint32_t temp = register_data_port.read();

	if ((temp & 0x8000) == 0x8000) print_str("GENERAL ERROR\n");
	if ((temp & 0x2000) == 0x2000) print_str("COLLISION ERROR\n");
	if ((temp & 0x1000) == 0x1000) print_str("MISSED FRAME ERROR\n");
	if ((temp & 0x0800) == 0x0800) print_str("MEMORY ERROR\n");
	if ((temp & 0x0400) == 0x0400) print_str("DATA RECEIVED\n");
	if ((temp & 0x0200) == 0x0200) print_str("DATA SENT\n");
	
	// acknoledge
	register_addr_port.write(0);
	register_data_port.write(temp);
	
	if ((temp & 0x0100) == 0x0200) print_str("INIT DONE\n");

	return esp;
}
