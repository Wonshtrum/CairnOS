#ifndef __DESKTOP_H__
#define __DESKTOP_H__

#include "utils/types.h"
#include "gui/color.h"
#include "gui/widget.h"
#include "gui/graphicsContext.h"
#include "drivers/mouse.h"


class Desktop: public Composite_widget, public Mouse_event_handler {
private:
	int32_t mouse_x;
	int32_t mouse_y;

public:
	Desktop(int32_t width, int32_t height, Color color);
	~Desktop();

	virtual void focus(Widget* widget) override;

	virtual void on_mouse_down(uint8_t button) override;
	virtual void on_mouse_up(uint8_t button) override;
	virtual void on_mouse_move(int32_t dx, int32_t dy) override;

	virtual void draw(Graphics_context* ctx) override;
	virtual void draw(Graphics_context* ctx, Bounding_box box) override;
};


#endif
