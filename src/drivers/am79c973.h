#ifndef __AM79C973_H__
#define __AM79C973_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/port.h"
#include "hardware/interrupts.h"
#include "hardware/pci.h"


class Driver_am79c973: public Driver, public Interrupt_handler {
private:
	struct Buffer_descriptor {
		uint32_t address;
		uint32_t flags_1;
		uint32_t flags_2;
		uint32_t avail;
	} __attribute__((packed));

	struct Initialization_block {
		uint16_t mode;
		uint8_t reserved_1 : 4;
		uint8_t num_send_buffers : 4;
		uint8_t reserved_2 : 4;
		uint8_t num_recv_buffers : 4;
		uint64_t physical_addr : 48;
		uint16_t reserved_3;
		uint64_t logical_addr;
		uint32_t recv_buffer_desc_addr;
		uint32_t send_buffer_desc_addr;
	} __attribute__((packed));

	Port_16bit mac_addr0_port;
	Port_16bit mac_addr2_port;
	Port_16bit mac_addr4_port;
	Port_16bit register_data_port;
	Port_16bit register_addr_port;
	Port_16bit reset_port;
	Port_16bit bus_controller_register_data_port;

	Initialization_block init;

	Buffer_descriptor* send_buffer_desc;
	uint8_t send_buffer_desc_mem[2*1024+15];
	uint8_t send_buffers[2*1024+15][8];
	uint8_t current_send_buffer;

	Buffer_descriptor* recv_buffer_desc;
	uint8_t recv_buffer_desc_mem[2*1024+15];
	uint8_t recv_buffers[2*1024+15][8];
	uint8_t current_recv_buffer;

public:
	Driver_am79c973(Device_descriptor* dev);
	~Driver_am79c973();

	virtual char* get_name() override;

	virtual void activate() override;
	virtual uint32_t reset() override;

	virtual uint32_t handle(uint32_t esp) override;

	void send(uint8_t* buffer, uint32_t size);
	void receive();
};


#endif
