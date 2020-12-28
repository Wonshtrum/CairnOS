#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "types.h"
#include "io.h"
#include "port.h"
#include "gdt.h"

#define IDT_DESC_PRESENT 0x80
#define IDT_INTERRUPT_GATE 0xE


class Interrupt_handler {
protected:
	uint8_t interrupt_number;

	Interrupt_handler(uint8_t interrupt_number);
	~Interrupt_handler();

public:
	virtual uint32_t handle(uint32_t esp);
};


class Interrupt_manager {
friend class Interrupt_handler;
protected:
	struct Gate_descriptor {
		uint16_t handler_addr_lo;
		uint16_t gdt_code_segment;
		uint8_t reserved;
		uint8_t access;
		uint16_t handler_addr_hi;
	} __attribute__((packed));

	static Gate_descriptor interrupt_descriptor_table[256];

	static void set_interrupt_descriptor_entry(
		uint8_t interrupt_number,
		uint16_t gdt_code_segment,
		void (*handler)(),
		uint8_t descriptor_privilege_level,
		uint8_t descriptor_type);

	struct Interrupt_descriptor_table_pointer {
		uint16_t size;
		uint32_t base;
	} __attribute__((packed));

	Port_8bit_slow pic_master_command;
	Port_8bit_slow pic_master_data;
	Port_8bit_slow pic_slave_command;
	Port_8bit_slow pic_slave_data;

	static Interrupt_manager* instance;

	Interrupt_handler* handlers[256];

public:
	Interrupt_manager(Global_descriptor_table* gdt);
	~Interrupt_manager();

	static Interrupt_manager* get();

	static void activate();

	static uint32_t handle_interrupt(uint8_t interrupt_number, uint32_t esp);
	uint32_t do_handle_interrupt(uint8_t interrupt_number, uint32_t esp);

	static void ignore_interrupt();
	static void handle_interrupt_0x00();
	static void handle_interrupt_0x01();
};


#endif
