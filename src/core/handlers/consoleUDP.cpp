#include "consoleUDP.h"


Console_UDP::Console_UDP():
	Keyboard_event_handler(),
	User_datagram_protocol_handler(),
	cursor(0) {}

void Console_UDP::on_key_down(char key) {
	if (key == SHIFT_KEY) {
		return;
	}
	if (key == DELETE_KEY) {
		cursor--;
		if (cursor < 0) {
			cursor = 0;
		}
		return;
	}
	buffer[cursor++] = key;
	print_chr(key);
	if (key == '\n' || cursor >= MAX_BUFFER_LENGTH) {
		print_str("sending...\n");
		send((uint8_t*)buffer, cursor);
		cursor = 0;
	}
}

void Console_UDP::on_receive(uint8_t* payload, uint32_t size) {
	for (uint32_t i = 0 ; i < size && payload[i] != '\0' ; i++) {
		print_chr(payload[i]);
	}
	print_str(" (");
	print_hex(size);
	print_str(")\n");
}
