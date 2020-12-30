#ifndef __PCI_H__
#define __PCI_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/port.h"
#include "hardware/interrupts.h"


struct Device_descriptor {
	uint32_t port_base;
	uint32_t interrupt;

	uint16_t bus;
	uint16_t device;
	uint16_t function;

	uint16_t vendor_id;
	uint16_t device_id;

	uint8_t class_id;
	uint8_t subclass_id;
	uint8_t interface_id;

	uint8_t revision;
};


class Peripheral_component_interconnect_controller {
private:
	Port_32bit data_port;
	Port_32bit command_port;

public:
	Peripheral_component_interconnect_controller();
	~Peripheral_component_interconnect_controller();

	uint32_t read(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset);
	void write(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset, uint32_t value);
	bool device_has_functions(uint16_t bus, uint16_t device);

	void select_drivers();
	Device_descriptor get_device_descriptor(uint16_t bus, uint16_t device, uint16_t function);
};


#endif
