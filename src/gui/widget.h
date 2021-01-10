#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "utils/types.h"
#include "utils/io.h"
#include "gui/graphicsContext.h"
#include "gui/boundingBox.h"
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
	bool transparent;

public:
	Widget(int32_t x, int32_t y, int32_t width, int32_t height, Color color, bool focussable = true);
	~Widget();

	Widget* get_parent();
	virtual Graphics_context* get_ctx();
	virtual void invalidate(Bounding_box boxes[], uint8_t n = 4);

	virtual void draw(Graphics_context* ctx);
	virtual void draw(Graphics_context* ctx, Bounding_box box);

	bool contains(int32_t x, int32_t y);
	void screen_offset(int32_t& dx, int32_t& dy);
	Bounding_box get_bounding_box();

	virtual void focus(Widget* widget);

	virtual void on_mouse_down(int32_t x, int32_t y, uint8_t button);
	virtual void on_mouse_up(int32_t x, int32_t y, uint8_t button);
	virtual void on_mouse_leave(int32_t x, int32_t y);
	virtual void on_mouse_enter(int32_t x, int32_t y);
	virtual void on_mouse_move(int32_t ox, int32_t oy, int32_t nx, int32_t ny);

	virtual void on_key_down(char key);
	virtual void on_key_up(char key);
};


class Composite_widget: public Widget {
private:
	Widget* children[MAX_NUM_SUB_WIDGETS];
	uint32_t num_children;

	Widget* focused;

public:
	Composite_widget(int32_t x, int32_t y, int32_t width, int32_t height, Color color, bool focussable = true);
	~Composite_widget();
	
	virtual void draw(Graphics_context* ctx) override;
	virtual void draw(Graphics_context* ctx, Bounding_box box) override;
	virtual void draw(Graphics_context* ctx, Bounding_box box, int32_t layer);

	bool add_child(Widget* widget);
	bool remove_child(Widget* widget);

	virtual void focus(Widget* widget) override;

	virtual void on_mouse_down(int32_t x, int32_t y, uint8_t button) override;
	virtual void on_mouse_up(int32_t x, int32_t y, uint8_t button) override;
	virtual void on_mouse_leave(int32_t x, int32_t y) override;
	virtual void on_mouse_enter(int32_t x, int32_t y) override;
	virtual void on_mouse_move(int32_t ox, int32_t oy, int32_t nx, int32_t ny) override;

	virtual void on_key_down(char key) override;
	virtual void on_key_up(char key) override;
};


#endif
