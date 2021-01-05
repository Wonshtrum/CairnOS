#include "interrupts.h"


Interrupt_manager::Gate_descriptor Interrupt_manager::interrupt_descriptor_table[256];
Interrupt_manager* Interrupt_manager::instance;
void Interrupt_manager::set_interrupt_descriptor_entry(
	uint8_t interrupt_number,
	uint16_t gdt_code_segment,
	void (*handler)(),
	uint8_t descriptor_privilege_level,
	uint8_t descriptor_type) {

	interrupt_descriptor_table[interrupt_number].handler_addr_lo = (((uint32_t)handler) >>  0) & 0xFFFF;
	interrupt_descriptor_table[interrupt_number].handler_addr_hi = (((uint32_t)handler) >> 16) & 0xFFFF;
	interrupt_descriptor_table[interrupt_number].gdt_code_segment = gdt_code_segment;
	interrupt_descriptor_table[interrupt_number].reserved = 0;
	interrupt_descriptor_table[interrupt_number].access = 0
		| IDT_DESC_PRESENT
		| descriptor_type
		| ((descriptor_privilege_level & 3) << 5);
}

Interrupt_manager::Interrupt_manager(Global_descriptor_table* gdt, Task_manager* task_manager):
	pic_master_command(0x20),
	pic_master_data(0x21),
	pic_slave_command(0xA0),
	pic_slave_data(0xA1),
	task_manager(task_manager) {

	instance = this;

	uint16_t code_segment = gdt->get_code_segment_selector();
	for (uint16_t interrupt_number = 0 ; interrupt_number < 256 ; interrupt_number++) {
		handlers[interrupt_number] = 0;
		set_interrupt_descriptor_entry(interrupt_number, code_segment, &ignore_interrupt, 0, IDT_INTERRUPT_GATE);
	}
	set_interrupt_descriptor_entry(0x00, code_segment, &handle_exception_0x00, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x01, code_segment, &handle_exception_0x01, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x02, code_segment, &handle_exception_0x02, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x03, code_segment, &handle_exception_0x03, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x04, code_segment, &handle_exception_0x04, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x05, code_segment, &handle_exception_0x05, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x06, code_segment, &handle_exception_0x06, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x07, code_segment, &handle_exception_0x07, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x08, code_segment, &handle_exception_0x08, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x09, code_segment, &handle_exception_0x09, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0A, code_segment, &handle_exception_0x0A, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0B, code_segment, &handle_exception_0x0B, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0C, code_segment, &handle_exception_0x0C, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0D, code_segment, &handle_exception_0x0D, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0E, code_segment, &handle_exception_0x0E, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0F, code_segment, &handle_exception_0x0F, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x10, code_segment, &handle_exception_0x10, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x11, code_segment, &handle_exception_0x11, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x12, code_segment, &handle_exception_0x12, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x13, code_segment, &handle_exception_0x13, 0, IDT_INTERRUPT_GATE);

	set_interrupt_descriptor_entry(0x00 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x00, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x01 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x01, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x02 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x02, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x03 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x03, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x04 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x04, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x05 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x05, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x06 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x06, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x07 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x07, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x08 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x08, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x09 + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x09, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0A + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x0A, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0B + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x0B, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0C + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x0C, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0D + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x0D, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0E + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x0E, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x0F + INTERRUPT_OFFSET, code_segment, &handle_interrupt_0x0F, 0, IDT_INTERRUPT_GATE);
	set_interrupt_descriptor_entry(0x80                   , code_segment, &handle_interrupt_0x80, 0, IDT_INTERRUPT_GATE);

	pic_master_command.write(0x11);	// Master init
	pic_slave_command.write(0x11);	// Slave init

	pic_master_data.write(0x20);	// Master PIC add 0x20 to any interrupt_number
	pic_slave_data.write(0x28);	// Slave PIC add 0x28 to any interrupt_number

	pic_master_data.write(0x04);
	pic_slave_data.write(0x02);

	pic_master_data.write(0x01);
	pic_slave_data.write(0x01);

	pic_master_data.write(0x00);
	pic_slave_data.write(0x00);


	Interrupt_descriptor_table_pointer idtp;
	idtp.size = 256 * sizeof(Gate_descriptor) - 1;
	idtp.base = (uint32_t)interrupt_descriptor_table;

	asm volatile("lidt %0" : : "m" (idtp));
}
Interrupt_manager::~Interrupt_manager() {}

Interrupt_manager* Interrupt_manager::get() {
	return instance;
}

void Interrupt_manager::activate() {
	asm("sti");
}

uint32_t Interrupt_manager::handle_interrupt(uint8_t interrupt_number, uint32_t esp) {
	return instance->do_handle_interrupt(interrupt_number, esp);
}

uint32_t Interrupt_manager::do_handle_interrupt(uint8_t interrupt_number, uint32_t esp) {
	if (handlers[interrupt_number] != 0) {
		esp = handlers[interrupt_number]->handle(esp);
	} else if (interrupt_number == INTERRUPT_OFFSET) {
		esp = (uint32_t)task_manager->shedule((CPU_state*)esp);
	} else {
		print_str("UNHANDLED INTERRUPT ");
		print_hex(interrupt_number);
		print_str("\n");
		while(1) {}
	}

	if (INTERRUPT_OFFSET <= interrupt_number && interrupt_number < INTERRUPT_OFFSET + 0x10) {
		pic_master_command.write(0x20);
		if (interrupt_number >= INTERRUPT_OFFSET + 0x08) {
			pic_slave_command.write(0x20);
		}
	}

	return esp;
}



Interrupt_handler::Interrupt_handler(uint8_t interrupt_number): interrupt_number(interrupt_number) {
	Interrupt_manager::get()->handlers[interrupt_number] = this;
}
Interrupt_handler::~Interrupt_handler() {
	if (Interrupt_manager::get()->handlers[interrupt_number] == this) {
		Interrupt_manager::get()->handlers[interrupt_number] = 0;
	}
}
uint32_t Interrupt_handler::handle(uint32_t esp) {}
