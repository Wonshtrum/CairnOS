#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "io.h"


class Keyboard_driver: public Interrupt_handler {
private:
	Port_8bit data_port;
	Port_8bit command_port;

public:
	Keyboard_driver();
	~Keyboard_driver();

	virtual uint32_t handle(uint32_t esp) override;
};


#endif
