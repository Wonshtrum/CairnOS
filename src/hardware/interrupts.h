#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/port.h"
#include "hardware/gdt.h"
#include "core/multitasking.h"

#define IDT_DESC_PRESENT 0x80
#define IDT_INTERRUPT_GATE 0xE
#define INTERRUPT_OFFSET 0x20

class Interrupt_handler {
protected:
	uint8_t interrupt_number;

	Interrupt_handler(uint8_t interrupt_number);
	~Interrupt_handler();

public:
	virtual void handle();
};


class Interrupt_manager {
friend class Interrupt_handler;
protected:
	static Interrupt_manager* instance;

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

	Interrupt_handler* handlers[256];

	Task_manager* task_manager;

public:
	Interrupt_manager(Global_descriptor_table* gdt, Task_manager* task_manager);
	~Interrupt_manager();

	static Interrupt_manager* get();

	static void activate();

	static uint32_t handle_interrupt(uint8_t interrupt_number, uint32_t esp);
	uint32_t do_handle_interrupt(uint8_t interrupt_number, uint32_t esp);

	static void ignore_interrupt();

	static void handle_exception_0x00();
	static void handle_exception_0x01();
	static void handle_exception_0x02();
	static void handle_exception_0x03();
	static void handle_exception_0x04();
	static void handle_exception_0x05();
	static void handle_exception_0x06();
	static void handle_exception_0x07();
	static void handle_exception_0x08();
	static void handle_exception_0x09();
	static void handle_exception_0x0A();
	static void handle_exception_0x0B();
	static void handle_exception_0x0C();
	static void handle_exception_0x0D();
	static void handle_exception_0x0E();
	static void handle_exception_0x0F();
	static void handle_exception_0x10();
	static void handle_exception_0x11();
	static void handle_exception_0x12();
	static void handle_exception_0x13();

	static void handle_interrupt_0x00();
	static void handle_interrupt_0x01();
	static void handle_interrupt_0x02();
	static void handle_interrupt_0x03();
	static void handle_interrupt_0x04();
	static void handle_interrupt_0x05();
	static void handle_interrupt_0x06();
	static void handle_interrupt_0x07();
	static void handle_interrupt_0x08();
	static void handle_interrupt_0x09();
	static void handle_interrupt_0x0A();
	static void handle_interrupt_0x0B();
	static void handle_interrupt_0x0C();
	static void handle_interrupt_0x0D();
	static void handle_interrupt_0x0E();
	static void handle_interrupt_0x0F();
};


#endif
