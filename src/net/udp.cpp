#include "udp.h"


UDP_socket::UDP_socket(User_datagram_protocol_provider* backend): backend(backend) {}
UDP_socket::~UDP_socket() {}

void UDP_socket::on_receive(uint8_t* payload, uint32_t size) {
	print_str("UDP from ");
	print_ip4(remote_ip);
	print_str(":");
	print_hex(remote_port, false);
	print_str("\n");
	for (uint32_t i = 0 ; i < size && i < 64 ; i++) {
		print_hex(payload[i], false);
	}
	print_str("\n");
}

void UDP_socket::send(uint8_t* buffer, uint32_t size) {
	backend->send(this, buffer, size);
}

void UDP_socket::disconnect() {
	backend->disconnect(this);
}


User_datagram_protocol_provider::User_datagram_protocol_provider(Internet_protocol_provider* backend):
	Internet_protocol_handler(backend, 0x11),
	num_sockets(0),
	free_port(1024) {

	for (uint32_t i = 0 ; i < (MAX_NUM_SOCKETS) ; i++) {
		sockets[i] = 0;
	}
}
User_datagram_protocol_provider::~User_datagram_protocol_provider() {}

bool User_datagram_protocol_provider::on_receive(uint32_t src_ip, uint32_t dst_ip, uint8_t* buffer, uint32_t size) {
	if (size < sizeof(UDP_frame)) {
		return false;
	}

	UDP_frame* frame = (UDP_frame*)buffer;
	for (uint32_t i = 0 ; i < num_sockets ; i++) {
		if (sockets[i]->local_port == frame->dst_port
		&&  sockets[i]->local_ip == dst_ip
		&&  sockets[i]->remote_port == frame->src_port
		&&  sockets[i]->remote_ip == src_ip) {
			sockets[i]->on_receive(buffer + sizeof(UDP_frame), size - sizeof(UDP_frame));
			break;
		}
	}

	return false;
}

UDP_socket* User_datagram_protocol_provider::connect(uint32_t ip, uint16_t port) {
	if (num_sockets >= MAX_NUM_SOCKETS) {
		return 0;
	}
	UDP_socket* socket = (UDP_socket*)malloc(sizeof(UDP_socket));
	if (socket == 0) {
		return 0;
	}
	*socket = UDP_socket(this);
	socket->remote_port = reverse_endian(port);
	socket->remote_ip = ip;
	socket->local_port = reverse_endian(free_port++);
	socket->local_ip = backend->get_ip();
	sockets[num_sockets++] = socket;

	return socket;
}

void User_datagram_protocol_provider::disconnect(UDP_socket* socket) {
	for (uint32_t i = 0 ; i < num_sockets ; i++) {
		if (sockets[i] == socket) {
			sockets[i] = sockets[--num_sockets];
			free(socket);
			break;
		}
	}
}

void User_datagram_protocol_provider::send(UDP_socket* socket, uint8_t* buffer, uint32_t size) {
	uint8_t* full_buffer = (uint8_t*)malloc(size + sizeof(UDP_frame));
	UDP_frame* frame = (UDP_frame*)full_buffer;
	frame->src_port = socket->local_port;
	frame->dst_port = socket->remote_port;
	frame->length = reverse_endian((uint16_t)(size + sizeof(UDP_frame)));
	frame->crc = 0;	// ignore check sum

	uint8_t* payload = full_buffer + sizeof(UDP_frame);
	for (uint32_t i = 0 ; i < size ; i++) {
		payload[i] = buffer[i];
	}

	Internet_protocol_handler::send(socket->remote_ip, full_buffer, size + sizeof(UDP_frame));
	free(full_buffer);
}
