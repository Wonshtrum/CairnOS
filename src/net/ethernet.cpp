#include "ethernet.h"


uint32_t make_ip(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4) {
	return ip4
	| (ip3 <<  8)
	| (ip2 << 16)
	| (ip1 << 24);
}
uint32_t make_ip_be(uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4) {
	return ip1
	| (ip2 <<  8)
	| (ip3 << 16)
	| (ip4 << 24);
}


Net_data_handler::Net_data_handler(Driver_ethernet* backend): backend(backend) {
	backend->set_handler(this);
}
Net_data_handler::~Net_data_handler() {
	backend->set_handler(0);
}

uint32_t Net_data_handler::get_ip() {
	return backend->get_ip();
}
uint64_t Net_data_handler::get_mac() {
	return backend->get_mac();
}
bool Net_data_handler::on_receive(uint8_t* buffer, uint32_t size) {
	return false;
}
void Net_data_handler::send(uint8_t* buffer, uint32_t size) {
	backend->send(buffer, size);
}


Driver_ethernet::Driver_ethernet(): Driver(), handler(0) {}
Driver_ethernet::~Driver_ethernet() {}

void Driver_ethernet::set_handler(Net_data_handler* handler) {
	this->handler = handler;
}

void Driver_ethernet::set_ip(uint32_t ip) {}
uint32_t Driver_ethernet::get_ip() { return 0; }
uint64_t Driver_ethernet::get_mac() { return 0; }
void Driver_ethernet::receive() {}
void Driver_ethernet::send(uint8_t* buffer, uint32_t size) {}
