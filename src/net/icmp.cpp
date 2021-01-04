#include "icmp.h"


Internet_control_message_protocol::Internet_control_message_protocol(Internet_protocol_provider* backend): Internet_protocol_handler(backend, 0x0001) {}
Internet_control_message_protocol::~Internet_control_message_protocol() {}

bool Internet_control_message_protocol::on_receive(uint32_t src_ip, uint32_t dst_ip, uint8_t* payload, uint32_t size) {
	if (size < sizeof(ICMP_frame)) {
		return false;
	}
	ICMP_frame* frame = (ICMP_frame*)payload;
	switch (frame->type) {
		case 0:	// ping response
			print_str("\nping response from ");
			print_ip4(src_ip);
			print_str(": ");
			print_hex(frame->data);
			print_str("\n");
			break;
		case 8:	// ping request
			frame->type = 0;
			frame->crc = 0;
			frame->crc = Internet_protocol_provider::check_sum((uint16_t*)&frame, sizeof(ICMP_frame));
			return true;
	}
	return false;
}

void Internet_control_message_protocol::echo_reply(uint32_t dst_ip) {
	ICMP_frame frame;
	frame.type = 8;	// ping
	frame.code = 0;
	frame.data = 0x3713;
	frame.crc = 0;
	frame.crc = Internet_protocol_provider::check_sum((uint16_t*)&frame, sizeof(ICMP_frame));

	send(dst_ip, (uint8_t*)&frame, sizeof(ICMP_frame));
}
