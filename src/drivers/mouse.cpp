#include "mouse.h"


Mouse_driver::Mouse_driver():
	Interrupt_handler(0x2C),
	data_port(0x60),
	command_port(0x64),
	offset(0),
	buttons(0),
	x(40),
	y(12) {

	command_port.write(0xA8);	// activate interrupts
	command_port.write(0x20);	// get current state

	uint8_t status = data_port.read() | 2;
	command_port.write(0x60);	// set state
	data_port.write(status);

	command_port.write(0xD4);
	data_port.write(0xF4);
	data_port.read();

	blink();
}
Mouse_driver::~Mouse_driver() {}

void Mouse_driver::blink() {
	static uint16_t* video_mem = (uint16_t*)0xb8000;
	video_mem[80*y+x] = 
		  ((video_mem[80*y+x] & 0xF000) >> 4)
		| ((video_mem[80*y+x] & 0x0F00) << 4)
		| ((video_mem[80*y+x] & 0x00FF));
}

uint32_t Mouse_driver::handle(uint32_t esp) {
	uint8_t status = command_port.read();
	if (!(status & 0x20)) {
		return esp;
	}

	buffer[offset] = data_port.read();
	offset = (offset + 1)%3;

	if (offset == 0) {
		blink();
		x += buffer[1];
		y -= buffer[2];
		if (x < 0) x = 0;
		if (x >= 80) x = 79;
		if (y < 0) y = 0;
		if (y >= 25) y = 24;
		blink();
	
		static char* labels[] = {"1", "2", "3"};
		for (uint8_t i = 0 ; i < 3 ; i++) {
			if ((buffer[0] & (1 << i)) != (buttons & (1 << i))) {
				print_str("Button ");
				print_str(labels[i]);
				print_str("\n");
			}
		}
		buttons = buffer[0];
	}

	return esp;
}
