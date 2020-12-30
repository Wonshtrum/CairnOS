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

				for (uint8_t bar_num = 0 ; bar_num < 6 ; bar_num++) {
					Base_address_register bar = get_base_address_register(bus, device, function, bar_num);
					if (bar.address && (bar.type == Input_output)) {
						device_descriptor.port_base = (uint32_t)bar.address;
					}
					Driver* driver = get_driver(device_descriptor);
				}

				print_str("\n  BUS ");
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

Driver* Peripheral_component_interconnect_controller::get_driver(Device_descriptor device_descriptor) {
	switch (device_descriptor.vendor_id) {
		case 0x1022:	// AMD
			print_str("AMD");
			switch (device_descriptor.device_id) {
				case 0x2000:	// am79c973
					print_str(":am79c973");
					break;
			}
			break;
		case 0x8086:	// Intel
			print_str("Intel");
			break;
	}
	switch (device_descriptor.class_id) {
		case 0x03:	// graphics
			print_str("Graphics");
			switch (device_descriptor.subclass_id) {
				case 0x00:	// VGA
					print_str(":VGA");
					break;
			}
			break;
	}
	print_str(" ");

	return 0;
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

Base_address_register Peripheral_component_interconnect_controller::get_base_address_register(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar_num) {
	Base_address_register result = {0};

	uint32_t header_type = read(bus, device, function, 0x0E) & 0x7F;
	uint8_t max_bars = 5 - (4*header_type);
	if (bar_num > max_bars) {
		return result;
	}

	uint32_t bar_value = read(bus, device, function, 0x10 + 4 * bar_num);
	result.type = (bar_value & 1) ? Input_output : Memory_mapping;

	if (result.type == Memory_mapping) {
		switch ((bar_value >> 1) & 0x3) {
			case 0b00: break;	// 32 bit mode
			case 0b01: break;	// 20 bit mode
			case 0b10: break;	// 64 bit mode
			default: break;
		}
		result.prefetchable = ((bar_value >> 3) & 1) == 1;

		uint32_t mask = ~0x7;
		write(bus, device, function, 0x10 + 4 * bar_num, mask);
		mask = read(bus, device, function, 0x10 + 4 * bar_num);
		write(bus, device, function, 0x10 + 4 * bar_num, bar_value);
	} else {
		result.address = (uint8_t*)(bar_value & ~0x3);
		result.prefetchable = false;
	}

	return result;
}
