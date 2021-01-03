#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/interrupts.h"
#include "hardware/port.h"
#include "drivers/driver.h"


class Keyboard_event_handler {
public:
	Keyboard_event_handler();
	~Keyboard_event_handler();
	
	virtual void on_activate();
	virtual void on_deactivate();
	virtual void on_key_down(char key);
	virtual void on_key_up(char key);
};


class Driver_keyboard: public Driver, public Interrupt_handler {
private:
	Port_8bit data_port;
	Port_8bit command_port;

	Keyboard_event_handler* handler;

public:
	Driver_keyboard(Keyboard_event_handler* handler = 0);
	~Driver_keyboard();
	
	virtual char* get_name() override;

	virtual void activate() override;
	virtual uint32_t handle(uint32_t esp) override;

	void set_event_handler(Keyboard_event_handler* handler);
};


#endif
