#ifndef __PRINT_KEYBOARD_H__
#define __PRINT_KEYBOARD_H__

#include "utils/types.h"
#include "utils/io.h"
#include "drivers/keyboard.h"


class Print_keyboard: public Keyboard_event_handler {
public:
	Print_keyboard();
	~Print_keyboard();

	virtual void on_key_down(char key) override;
	virtual void on_key_up(char key) override;
};


#endif
