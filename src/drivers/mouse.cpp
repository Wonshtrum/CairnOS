#include "mouse.h"


Mouse_event_handler::Mouse_event_handler() {}
Mouse_event_handler::~Mouse_event_handler() {}
void Mouse_event_handler::on_activate() {}
void Mouse_event_handler::on_deactivate() {}
void Mouse_event_handler::on_mouse_down(uint8_t button) {}
void Mouse_event_handler::on_mouse_up(uint8_t button) {}
void Mouse_event_handler::on_mouse_move(int32_t x, int32_t y) {}


Driver_mouse::Driver_mouse(Mouse_event_handler* handler):
	Driver(),
	Interrupt_handler(0x2C),
	data_port(0x60),
	command_port(0x64),
	offset(0),
	buttons(0),
	handler(handler) {}
Driver_mouse::~Driver_mouse() {}

char* Driver_mouse::get_name() { return "Mouse"; }

void Driver_mouse::activate() {
	command_port.write(0xA8);	// activate interrupts
	command_port.write(0x20);	// get current state

	uint8_t status = data_port.read() | 2;
	command_port.write(0x60);	// set state
	data_port.write(status);

	command_port.write(0xD4);
	data_port.write(0xF4);
	data_port.read();

	activated = true;
	if (handler != 0) {
		handler->on_activate();
	}
}

void Driver_mouse::handle() {
	uint8_t status = command_port.read();
	if (!(status & 0x20)) {
		return;
	}

	buffer[offset] = data_port.read();
	offset = (offset + 1) % 3;

	if (handler == 0) {
		return;
	}

	if (offset == 0) {
		if (buffer[1] != 0 || buffer[2] != 0) {
			handler->on_mouse_move((int8_t)buffer[1], -((int8_t)buffer[2]));
		}
		for (uint8_t i = 0 ; i < 3 ; i++) {
			if ((buffer[0] & (1 << i)) != (buttons & (1 << i))) {
				if (buffer[0] & (1 << i)) {
					handler->on_mouse_down(i+1);
				} else {
					handler->on_mouse_up(i+1);
				}
			}
		}
		buttons = buffer[0];
	}
}

void Driver_mouse::set_event_handler(Mouse_event_handler* handler) {
	if (this->handler != 0) {
		this->handler->on_deactivate();
	}
	this->handler = handler;
	if (activated && handler != 0) {
		handler->on_activate();
	}
}
