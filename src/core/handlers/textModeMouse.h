#ifndef __TEXT_MODE_MOUSE__
#define __TEXT_MODE_MOUSE__

#include "utils/types.h"
#include "utils/io.h"
#include "drivers/mouse.h"


class Text_mode_mouse: public Mouse_event_handler {
private:
	int32_t width;
	int32_t height;

	int32_t x;
	int32_t y;

	void blink();

public:
	Text_mode_mouse(int32_t width, int32_t height);
	~Text_mode_mouse();

	virtual void on_activate() override;
	virtual void on_deactivate() override;
	virtual void on_mouse_down(uint8_t button) override;
	virtual void on_mouse_up(uint8_t button) override;
	virtual void on_mouse_move(int32_t x, int32_t y) override;
};


#endif
