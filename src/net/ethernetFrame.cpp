#include "ethernetFrame.h"


Ethernet_frame_handler::Ethernet_frame_handler(Ethernet_frame_provider* backend, uint16_t ether_type) {
	this->ether_type = 0
		| ((ether_type & 0xFF00) >> 8)
		| ((ether_type & 0x00FF) << 8);
	backend->add_handler(this);
	this->backend = backend;
}
Ethernet_frame_handler::~Ethernet_frame_handler() {
	backend->handlers[ether_type] = 0;
}

bool Ethernet_frame_handler::on_receive(uint8_t* payload, uint32_t size) {
	return false;
}
void Ethernet_frame_handler::send(uint64_t dst_mac, uint8_t* payload, uint32_t size) {
	backend->send(dst_mac, ether_type, payload, size);
}

uint16_t Ethernet_frame_handler::get_ether_type() {
	return ether_type;
}


Ethernet_frame_provider::Ethernet_frame_provider(Driver_ethernet* backend): Net_data_handler(backend) {}
Ethernet_frame_provider::~Ethernet_frame_provider() {}

bool Ethernet_frame_provider::on_receive(uint8_t* buffer, uint32_t size) {
	Ethernet_frame_header* frame = (Ethernet_frame_header*)buffer;
	bool send_back = false;
	if (frame->dst_mac == MAC_BROADCAST || frame->dst_mac == backend->get_mac()) {
		bool handled = false;
		for (uint16_t i = 0 ; i < num_handlers ; i++) {
			if (handlers[i]->get_ether_type() == frame->ether_type) {
				send_back = handlers[i]->on_receive(buffer + sizeof(Ethernet_frame_header), size - sizeof(Ethernet_frame_header));
				handled = true;
				break;
			}
		}
		if (!handled) {
			print_str("Ethernet message unhandled with etherType = ");
			print_hex(frame->ether_type);
			print_str("\n");
		}
	}
	if (send_back) {
		frame->dst_mac = frame->src_mac;
		frame->src_mac = backend->get_mac();
	}
	return send_back;
}

void Ethernet_frame_provider::send(uint64_t dst_mac, uint16_t ether_type, uint8_t* buffer, uint32_t size) {
	uint8_t* full_buffer = (uint8_t*)malloc(sizeof(Ethernet_frame_header) + size);
	Ethernet_frame_header* frame = (Ethernet_frame_header*) full_buffer;
	frame->dst_mac = dst_mac;
	frame->src_mac = backend->get_mac();
	frame->ether_type = ether_type;

	uint8_t* payload = full_buffer + sizeof(Ethernet_frame_header);
	for (uint32_t i = 0 ; i < size ; i++) {
		payload[i] = buffer[i];
	}

	backend->send(full_buffer, sizeof(Ethernet_frame_header) + size);
}

bool Ethernet_frame_provider::add_handler(Ethernet_frame_handler* handler) {
	if (num_handlers >= MAX_ETHER_TYPE_HANDLERS) {
		return false;
	}
	handlers[num_handlers++] = handler;
	return true;
}
