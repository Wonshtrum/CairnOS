#ifndef __ATA_H__
#define __ATA_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/port.h"
#include "drivers/driver.h"


class Driver_ATA {
protected:
	Port_16bit data_port;
	Port_8bit error_port;
	Port_8bit sector_count_port;
	Port_8bit lba_lo_port;
	Port_8bit lba_mi_port;
	Port_8bit lba_hi_port;
	Port_8bit device_port;
	Port_8bit command_port;
	Port_8bit control_port;

	bool master;
	uint16_t bytes_per_sector;

public:
	Driver_ATA(uint16_t port_base, bool master);
	~Driver_ATA();

	void identify();
	void read_28(uint32_t sector, uint8_t* data, uint32_t count);
	void write_28(uint32_t sector, uint8_t* data, uint32_t count);
	void flush();
};


#endif
