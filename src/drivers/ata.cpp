#include "ata.h"


Driver_ATA::Driver_ATA(uint16_t port_base, bool master):
	data_port(port_base),
	error_port(port_base + 1),
	sector_count_port(port_base + 2),
	lba_lo_port(port_base + 3),
	lba_mi_port(port_base + 4),
	lba_hi_port(port_base + 5),
	device_port(port_base + 6),
	command_port(port_base + 7),
	control_port(port_base + 0x206),
	bytes_per_sector(512),
	master(master) {}
Driver_ATA::~Driver_ATA() {}

void Driver_ATA::identify() {
	device_port.write(master ? 0xA0 : 0xB0);
	control_port.write(0);

	device_port.write(0xA0);
	uint8_t status = command_port.read();
	if (status == 0xFF) {
		print_str("NO DEVICE");
		return;
	}

	device_port.write(master ? 0xA0 : 0xB0);
	sector_count_port.write(0);
	lba_lo_port.write(0);
	lba_mi_port.write(0);
	lba_hi_port.write(0);
	command_port.write(0xEC);

	status = command_port.read();
	if (status == 0x00) {
		print_str("NO DEVICE");
		return;
	}

	// while HD busy and no error
	while ((status & 0x81) == 0x80) {
		status = command_port.read();
	}

	if (status & 0x01) {
		print_str("HD ERROR");
		return;
	}

	for (uint16_t i = 0 ; i < 256 ; i++) {
		uint16_t data = data_port.read();
		//print_chr((char)((data >> 8) & 0xFF));
		//print_chr((char)((data >> 0) & 0xFF));
	}
	print_str("OK");
}

void Driver_ATA::read_28(uint32_t sector, uint8_t* data, uint32_t count) {
	if (sector & 0xF0000000) {
		return;	// can't address more than 28 bits
	}
	if (count > bytes_per_sector) {
		return; // to much data
	}
	
	print_str("READING FROM ATA: ");

	device_port.write((master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
	error_port.write(0);
	sector_count_port.write(1);

	lba_lo_port.write((sector & 0x000000FF) >>  0);
	lba_mi_port.write((sector & 0x0000FF00) >>  8);
	lba_hi_port.write((sector & 0x00FF0000) >> 16);
	command_port.write(0x20);

	// while HD busy and no error
	uint8_t status = command_port.read();
	while ((status & 0x81) == 0x80) {
		status = command_port.read();
	}

	if (status & 0x01) {
		print_str("HD ERROR");
		return;
	}

	for (uint16_t i = 0 ; i < count ; i += 2) {
		uint16_t read_data = data_port.read();

		data[i]   = (uint8_t)(read_data & 0xFF);
		print_chr((char)(read_data & 0xFF));
		if (i + 1 < count) {
			data[i+1] = (uint8_t)((read_data >> 8) & 0xFF);
			print_chr((char)((read_data >> 8) & 0xFF));
		}
	}
	print_str("\n");

	for (uint16_t i = count + (count % 2) ; i < bytes_per_sector ; i += 2) {
		data_port.read();
	}
}

void Driver_ATA::write_28(uint32_t sector, uint8_t* data, uint32_t count) {
	if (sector & 0xF0000000) {
		return;	// can't address more than 28 bits
	}
	if (count > bytes_per_sector) {
		return; // to much data
	}
	
	print_str("WRITING TO ATA: ");

	device_port.write((master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
	error_port.write(0);
	sector_count_port.write(1);

	lba_lo_port.write((sector & 0x000000FF) >>  0);
	lba_mi_port.write((sector & 0x0000FF00) >>  8);
	lba_hi_port.write((sector & 0x00FF0000) >> 16);
	command_port.write(0x30);

	for (uint16_t i = 0 ; i < count ; i += 2) {
		uint16_t write_data = data[i];
		if (i + 1 < count) {
			write_data |= ((uint16_t)data[i+1]) << 8;
		}
		data_port.write(write_data);

		print_chr((char)((write_data >> 0) & 0xFF));
		print_chr((char)((write_data >> 8) & 0xFF));
	}
	print_str("\n");

	for (uint16_t i = count + (count % 2) ; i < bytes_per_sector ; i += 2) {
		data_port.write(0x0000);
	}
}

void Driver_ATA::flush() {
	device_port.write(master ? 0xE0 : 0xF0);
	command_port.write(0xE7);

	// while HD busy and no error
	uint8_t status = command_port.read();
	while ((status & 0x81) == 0x80) {
		status = command_port.read();
	}

	if (status & 0x01) {
		print_str("HD ERROR");
		return;
	}
	print_str("ATA FLUSHED\n");
}
