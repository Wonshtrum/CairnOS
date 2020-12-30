#include "pci.h"


Peripheral_component_interconnect_controller::Peripheral_component_interconnect_controller():
	data_port(0xCFC),
	command_port(0xCF8) {}

Peripheral_component_interconnect_controller::~Peripheral_component_interconnect_controller() {}

uint32_t Peripheral_component_interconnect_controller::read(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset) {
	uint32_t id =
		1 << 31
		| ((bus & 0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (register_offset & 0xFC);	// 4 byte aligned
	command_port.write(id);
	return data_port.read() >> (8 * (register_offset % 4));
}

void Peripheral_component_interconnect_controller::write(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset, uint32_t value) {
	uint32_t id =
		1 << 31
		| ((bus & 0xFF) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (register_offset & 0xFC);
	command_port.write(id);
	data_port.write(value);
}

bool Peripheral_component_interconnect_controller::device_has_functions(uint16_t bus, uint16_t device) {
	return read(bus, device, 0, 0x0E) & (1 << 7);
}

void Peripheral_component_interconnect_controller::select_drivers() {
	print_str("\n");
	for (uint8_t bus = 0 ; bus < 8 ; bus++) {
		for (uint8_t device = 0 ; device < 32 ; device++) {
			uint8_t num_functions = device_has_functions(bus, device) ? 8 : 1;
			for (uint8_t function = 0 ; function < num_functions ; function++) {
				Device_descriptor device_descriptor = get_device_descriptor(bus, device, function);

				if (device_descriptor.vendor_id == 0x0000 || device_descriptor.vendor_id == 0xFFFF) {
					continue;
				}

				print_str("BUS ");
				print_hex(bus);
				print_str(", DEV ");
				print_hex(device);
				print_str(", FUN ");
				print_hex(function);
				print_str(", VEN");
				print_hex(device_descriptor.vendor_id);
				print_str(", DEV");
				print_hex(device_descriptor.device_id);
				print_str(", INT");
				print_hex(device_descriptor.interrupt);
				print_str("\n");
			}
		}
	}
	print_str("\n");
}

Device_descriptor Peripheral_component_interconnect_controller::get_device_descriptor(uint16_t bus, uint16_t device, uint16_t function) {
	Device_descriptor result;
	result.bus = bus;
	result.device = device;
	result.function = function;

	result.vendor_id = read(bus, device, function, 0x00);
	result.device_id = read(bus, device, function, 0x02);

	result.class_id = read(bus, device, function, 0x0B);
	result.subclass_id = read(bus, device, function, 0x0A);
	result.interface_id = read(bus, device, function, 0x09);

	result.revision = read(bus, device, function, 0x08);
	result.interrupt = read(bus, device, function, 0x3C);

	return result;
}
