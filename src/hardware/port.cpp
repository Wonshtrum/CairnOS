#include "port.h"


Port::Port(uint16_t port_number): port_number(port_number) {}
Port::~Port() {}


Port_8bit::Port_8bit(uint16_t port_number): Port(port_number) {}
Port_8bit::~Port_8bit() {}

void Port_8bit::write(uint8_t data) {
	asm volatile("outb %0, %1" : : "a" (data), "Nd" (port_number));
}
uint8_t Port_8bit::read() {
	uint8_t result;
	asm volatile("inb %1, %0" : "=a" (result) : "Nd" (port_number));
	return result;
}


Port_8bit_slow::Port_8bit_slow(uint16_t port_number): Port_8bit(port_number) {}
Port_8bit_slow::~Port_8bit_slow() {}

void Port_8bit_slow::write(uint8_t data) {
	asm volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (port_number));
}


Port_16bit::Port_16bit(uint16_t port_number): Port(port_number) {}
Port_16bit::~Port_16bit() {}

void Port_16bit::write(uint16_t data) {
	asm volatile("outw %0, %1" : : "a" (data), "Nd" (port_number));
}
uint16_t Port_16bit::read() {
	uint16_t result;
	asm volatile("inw %1, %0" : "=a" (result) : "Nd" (port_number));
	return result;
}


Port_32bit::Port_32bit(uint16_t port_number): Port(port_number) {}
Port_32bit::~Port_32bit() {}

void Port_32bit::write(uint32_t data) {
	asm volatile("outl %0, %1" : : "a" (data), "Nd" (port_number));
}
uint32_t Port_32bit::read() {
	uint32_t result;
	asm volatile("inl %1, %0" : "=a" (result) : "Nd" (port_number));
	return result;
}
