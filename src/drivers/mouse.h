#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/interrupts.h"
#include "hardware/port.h"
#include "drivers/driver.h"


class Mouse_event_handler {
public:
	Mouse_event_handler();
	~Mouse_event_handler();
	
	virtual void on_activate();
	virtual void on_deactivate();
	virtual void on_mouse_down(uint8_t button);
	virtual void on_mouse_up(uint8_t button);
	virtual void on_mouse_move(int32_t x, int32_t y);
};


class Driver_mouse: public Driver, public Interrupt_handler {
private:
	Port_8bit data_port;
	Port_8bit command_port;

	uint8_t buffer[3];
	uint8_t offset;
	uint8_t buttons;

	Mouse_event_handler* handler;

public:
	Driver_mouse(Mouse_event_handler* handler = 0);
	~Driver_mouse();

	virtual char* get_name() override;

	virtual uint32_t handle(uint32_t esp) override;
	virtual void activate() override;
	
	void set_event_handler(Mouse_event_handler* handler);
};


#endif
