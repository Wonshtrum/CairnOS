#include "ethernet.h"


Net_data_handler::Net_data_handler(Driver_ethernet* backend): backend(backend) {
	backend->set_handler(this);
}
Net_data_handler::~Net_data_handler() {
	backend->set_handler(0);
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

uint64_t Driver_ethernet::get_mac() { return 0; }
void Driver_ethernet::receive() {}
void Driver_ethernet::send(uint8_t* buffer, uint32_t size) {}
