#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/interrupts.h"
#include "hardware/port.h"


class Mouse_driver: public Interrupt_handler {
private:
	Port_8bit data_port;
	Port_8bit command_port;

	uint8_t buffer[3];
	uint8_t offset;
	uint8_t buttons;

	int8_t x;
	int8_t y;

	void blink();

public:
	Mouse_driver();
	~Mouse_driver();

	virtual uint32_t handle(uint32_t esp) override;
};


#endif
