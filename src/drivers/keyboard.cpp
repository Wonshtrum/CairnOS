#include "keyboard.h"


Keyboard_driver::Keyboard_driver():
	Interrupt_handler(0x21),
	data_port(0x60),
	command_port(0x64) {

	while (command_port.read() & 0x1) {
		data_port.read();
	}
	command_port.write(0xAE);	// activate interrupts
	command_port.write(0x20);	// get current state

	uint8_t status = (data_port.read() | 1) & ~0x10;
	command_port.write(0x60);	// set state
	data_port.write(status);

	data_port.write(0xF4);
}
Keyboard_driver::~Keyboard_driver() {}

uint32_t Keyboard_driver::handle(uint32_t esp) {
	uint8_t key = data_port.read();
	bool pressed = (key & 0x80) == 0;
	if (pressed) {
		print_str(" P");
	} else {
		print_str(" R");
	}
	key &= 0x7F;
	print_hex(key);
	return esp;
}
