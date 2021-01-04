#include "ipv4.h"


Internet_protocol_handler::Internet_protocol_handler(Internet_protocol_provider* backend, uint8_t protocol) {
	this->protocol = protocol;
	backend->handlers[protocol] = this;
	this->backend = backend;
}
Internet_protocol_handler::~Internet_protocol_handler() {
	if (backend->handlers[protocol] == this) {
		backend->handlers[protocol] = 0;
	}
}

bool Internet_protocol_handler::on_receive(uint32_t src_ip, uint32_t dst_ip, uint8_t* payload, uint32_t size) { return false; }
void Internet_protocol_handler::send(uint32_t dst_ip, uint8_t* payload, uint32_t size) {
	backend->send(dst_ip, protocol, payload, size);
}


Internet_protocol_provider::Internet_protocol_provider(Ethernet_frame_provider* backend, Address_resolution_protocol* arp, uint32_t gw_ip, uint32_t mask):
	Ethernet_frame_handler(backend, 0x0800),
	arp(arp),
	gw_ip(gw_ip),
	mask(mask) {
	
	for (uint32_t i = 0 ; i < 256 ; i++) {
		handlers[i] = 0;
	}
}
Internet_protocol_provider::~Internet_protocol_provider() {}

bool Internet_protocol_provider::on_receive(uint8_t* buffer, uint32_t size) {
	if (size < sizeof(IPv4_frame)) {
		return false;
	}

	IPv4_frame* frame = (IPv4_frame*)buffer;
	bool send_back = false;
	uint32_t header_length = 4*frame->header_length;

	if (frame->dst_ip == backend->get_ip()) {
		int32_t length = frame->total_length;
		if (length > size) {
			length = size;
		}

		if (handlers[frame->protocol] != 0) {
			send_back = handlers[frame->protocol]->on_receive(frame->src_ip, frame->dst_ip, buffer + header_length, length - header_length);
		}
	}

	if (send_back) {
		frame->dst_ip = frame->src_ip;
		frame->src_ip = backend->get_ip();

		frame->ttl = 0x40;
		frame->crc = 0;
		frame->crc = check_sum((uint16_t*)frame, header_length);
	}

	return send_back;
}

void Internet_protocol_provider::send(uint32_t dst_ip, uint8_t protocol, uint8_t* buffer, uint32_t size) {
	uint8_t* full_buffer = (uint8_t*)malloc(sizeof(IPv4_frame) + size);
	IPv4_frame* frame = (IPv4_frame*)full_buffer;
	frame->version = 4;
	frame->header_length = sizeof(IPv4_frame)/4;
	frame->tos = 0;
	frame->total_length = reverse_endian(sizeof(IPv4_frame) + size);
	frame->identification = 0x0100;	// random
	frame->flags = 0x00;		// not fragmented
	frame->offset = 0x00;		// first fragment
	frame->ttl = 0x40;
	frame->protocol = protocol;
	frame->dst_ip = dst_ip;
	frame->src_ip = backend->get_ip();
	frame->crc = 0;
	frame->crc = check_sum((uint16_t*)frame, sizeof(IPv4_frame));

	uint8_t* payload = full_buffer + sizeof(IPv4_frame);
	for (uint32_t i = 0 ; i < size ; i++) {
		payload[i] = buffer[i];
	}

	uint32_t route = dst_ip;
	if ((dst_ip & mask) != (backend->get_ip() & mask)) {
		route = gw_ip;
	}
	backend->send(arp->resolve(route), ether_type, full_buffer, sizeof(IPv4_frame) + size);
	free(full_buffer);
}

uint16_t Internet_protocol_provider::check_sum(uint16_t* data, uint32_t size) {
	uint32_t tmp = 0;
	for (uint32_t i = 0 ; i < size/2 ; i++) {
		tmp += reverse_endian(data[i]);
	}
	if (size % 2) {
		tmp += reverse_endian((uint16_t)(((uint8_t*)data)[size-1]));
	}
	while (tmp & 0xFFFF0000) {
		tmp = (tmp & 0xFFFF) + (tmp >> 16);
	}
	return reverse_endian(~tmp);
}
