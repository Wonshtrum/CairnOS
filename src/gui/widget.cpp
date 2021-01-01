#include "widget.h"


Widget::Widget(int32_t x, int32_t y, int32_t width, int32_t height, Color color, bool focussable):
	parent(0),
	focussable(focussable),
	pos_x(x),
	pos_y(y),
	width(width),
	height(height),
	color(color) {}
Widget::~Widget() {}

void Widget::focus() {}
void Widget::on_mouse_down(uint8_t button) {}
void Widget::on_mouse_up(uint8_t button) {}
void Widget::on_mouse_leave(int32_t x, int32_t y) {}
void Widget::on_mouse_enter(int32_t x, int32_t y) {}
void Widget::on_mouse_move(int32_t x, int32_t y) {}

Widget* Widget::get_parent() {
	return parent;
}

void Widget::screen_offset(int32_t& dx, int32_t& dy) {
	dx += pos_x;
	dy += pos_y;
	if (parent == 0) {
		return;
	}
	parent->screen_offset(dx, dy);
}

void Widget::draw(Graphics_context* ctx) {
	int32_t dx = 0, dy = 0;
	screen_offset(dx, dy);
	for (int32_t x = 0 ; x < width ; x++) {
		for (int32_t y = 0 ; y < height ; y++) {
			ctx->put_pixel(x + dx, y + dy, color);
		}
	}
}

Composite_widget::Composite_widget(int32_t x, int32_t y, int32_t width, int32_t height, Color color, bool focussable):
	Widget(x, y, width, height, color),
	num_children(0) {}
Composite_widget::~Composite_widget() {}

bool Composite_widget::add_child(Widget* widget) {
	if (num_children >= MAX_NUM_SUB_WIDGETS) {
		return false;
	}

	children[num_children++] = widget;
	if (widget->parent != 0) {
		widget->parent->remove_child(widget);
	}
	widget->parent = this;
	return true;
}

bool Composite_widget::remove_child(Widget* widget) {
	for (uint32_t i = 0 ; i < num_children ; i++) {
		if (children[i] == widget) {
			num_children--;
			for (uint32_t j = i ; j < num_children ; j++) {
				children[j] = children[j+1];
			}
			return true;
		}
	}
	return false;
}

void Composite_widget::draw(Graphics_context* ctx) {
	Widget::draw(ctx);
	for (uint32_t i = 0 ; i < num_children ; i++) {
		children[i]->draw(ctx);
	}
}
