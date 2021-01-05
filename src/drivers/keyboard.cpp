#include "keyboard.h"


Keyboard_event_handler::Keyboard_event_handler() {}
Keyboard_event_handler::~Keyboard_event_handler() {}
void Keyboard_event_handler::on_activate() {}
void Keyboard_event_handler::on_deactivate() {}
void Keyboard_event_handler::on_key_down(char key) {}
void Keyboard_event_handler::on_key_up(char key) {}


Driver_keyboard::Driver_keyboard(Keyboard_event_handler* handler):
	Driver(),
	Interrupt_handler(0x21),
	data_port(0x60),
	command_port(0x64),
	handler(handler) {}
Driver_keyboard::~Driver_keyboard() {}

char* Driver_keyboard::get_name() { return "Keyboard"; }

void Driver_keyboard::activate() {
	while (command_port.read() & 0x1) {
		data_port.read();
	}
	command_port.write(0xAE);	// activate interrupts
	command_port.write(0x20);	// get current state

	uint8_t status = (data_port.read() | 1) & ~0x10;
	command_port.write(0x60);	// set state
	data_port.write(status);

	data_port.write(0xF4);

	activated = true;
	if (handler == 0) {
		handler->on_activate();
	}
}

uint32_t Driver_keyboard::handle(uint32_t esp) {
	uint8_t code = data_port.read();

	if (handler == 0) {
		return esp;
	}

	static bool shift = false;
	bool pressed = (code & 0x80) == 0;
	char key = decode_key(code & 0x7F, shift);
	//if (key == '_') print_hex((uint8_t)(code & 0x7F));

	if (key == SHIFT_KEY) {
		shift = pressed;
	}
	if (pressed) {
		handler->on_key_down(key);
	} else {
		handler->on_key_up(key);
	}

	return esp;
}

void Driver_keyboard::set_event_handler(Keyboard_event_handler* handler) {
	if (this->handler != 0) {
		this->handler->on_deactivate();
	}
	this->handler = handler;
	if (activated && handler != 0) {
		handler->on_activate();
	}
}
