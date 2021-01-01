#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "utils/types.h"
#include "utils/io.h"
#include "gui/graphicsContext.h"
#define MAX_NUM_SUB_WIDGETS 100

class Composite_widget;

class Widget {
friend class Composite_widget;
protected:
	Composite_widget* parent;

	bool focussable;

	int32_t pos_x;
	int32_t pos_y;
	int32_t width;
	int32_t height;
	Color color;

public:
	Widget(int32_t x, int32_t y, int32_t width, int32_t height, Color color, bool focussable = false);
	~Widget();

	Widget* get_parent();

	virtual void draw(Graphics_context* ctx);
	bool contains(int32_t x, int32_t y);
	void screen_offset(int32_t& dx, int32_t& dy);

	virtual void focus();
	virtual void on_mouse_down(uint8_t button);
	virtual void on_mouse_up(uint8_t button);
	virtual void on_mouse_leave(int32_t x, int32_t y);
	virtual void on_mouse_enter(int32_t x, int32_t y);
	virtual void on_mouse_move(int32_t x, int32_t y);
};


class Composite_widget: public Widget {
private:
	Widget* children[MAX_NUM_SUB_WIDGETS];
	uint32_t num_children;

public:
	Composite_widget(int32_t x, int32_t y, int32_t width, int32_t height, Color color, bool focussable = false);
	~Composite_widget();

	virtual void draw(Graphics_context* ctx) override;

	bool add_child(Widget* widget);
	bool remove_child(Widget* widget);
};


#endif
