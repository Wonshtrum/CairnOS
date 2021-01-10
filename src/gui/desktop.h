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

	Graphics_context* ctx;

public:
	Desktop(int32_t width, int32_t height, Color color);
	~Desktop();

	void set_ctx(Graphics_context* ctx);
	virtual Graphics_context* get_ctx() override;
	virtual void invalidate(Bounding_box boxes[], uint8_t n = 4) override;

	virtual void focus(Widget* widget) override;

	virtual void on_mouse_down(uint8_t button) override;
	virtual void on_mouse_up(uint8_t button) override;
	virtual void on_mouse_move(int32_t dx, int32_t dy) override;

	void erase_mouse(Graphics_context* ctx, int32_t x, int32_t y);
	void draw_mouse(Graphics_context* ctx, int32_t x, int32_t y);
};


#endif
