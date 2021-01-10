#include "widget.h"


Widget::Widget(int32_t x, int32_t y, int32_t width, int32_t height, Color color, bool focussable):
	parent(0),
	focussable(focussable),
	pos_x(x),
	pos_y(y),
	width(width),
	height(height),
	color(color),
	transparent(false) {}
Widget::~Widget() {}

bool Widget::contains(int32_t x, int32_t y) {
	return x >= pos_x && y >= pos_y && x <= pos_x + width && y <= pos_y + height;
}

Bounding_box Widget::get_bounding_box() {
	int32_t dx = 0, dy = 0;
	screen_offset(dx, dy);
	return { dx, dy, width, height };
}

Graphics_context* Widget::get_ctx() {
	if (parent != 0) {
		return parent->get_ctx();
	}
	return 0;
}

void Widget::invalidate(Bounding_box boxes[], uint8_t n) {
	if (parent != 0) {
		parent->invalidate(boxes, n);
	}
}

void Widget::focus(Widget* widget) {
	if (parent != 0) {
		parent->focus(widget);
	}
}
void Widget::on_mouse_down(int32_t x, int32_t y, uint8_t button) {
	if (focussable) {
		focus(this);
	}
}
void Widget::on_mouse_up(int32_t x, int32_t y, uint8_t button) {}
void Widget::on_mouse_leave(int32_t x, int32_t y) {}
void Widget::on_mouse_enter(int32_t x, int32_t y) {}
void Widget::on_mouse_move(int32_t ox, int32_t oy, int32_t nx, int32_t ny) {}
void Widget::on_key_down(char key) {}
void Widget::on_key_up(char key) {}

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
	if (transparent) {
		return;
	}
	int32_t dx = 0, dy = 0;
	screen_offset(dx, dy);
	ctx->fill_rectangle(dx, dy, width, height, color);
}
void Widget::draw(Graphics_context* ctx, Bounding_box box) {
	if (transparent) {
		return;
	}
	int32_t dx = 0, dy = 0;
	screen_offset(dx, dy);
	Bounding_box inner = intersect(box, { dx, dy, width, height });
	ctx->fill_rectangle(inner.pos_x, inner.pos_y, inner.width, inner.height, color);
}

Composite_widget::Composite_widget(int32_t x, int32_t y, int32_t width, int32_t height, Color color, bool focussable):
	Widget(x, y, width, height, color),
	num_children(0),
	focused(0) {}
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
	for (int32_t i = 0 ; i < num_children ; i++) {
		if (children[i] == widget) {
			num_children--;
			for (int32_t j = i ; j < num_children ; j++) {
				children[j] = children[j+1];
			}
			return true;
		}
	}
	return false;
}

void Composite_widget::draw(Graphics_context* ctx) {
	if (transparent) {
		return;
	}
	Widget::draw(ctx);
	for (int32_t i = 0 ; i < num_children ; i++) {
		children[i]->draw(ctx);
	}
}
void Composite_widget::draw(Graphics_context* ctx, Bounding_box box) {
	return draw(ctx, box, num_children-1);
}
void Composite_widget::draw(Graphics_context* ctx, Bounding_box box, int32_t layer) {
	if (layer < 0) {
		Widget::draw(ctx, box);
		return;
	}
	if (children[layer]->transparent) {
		draw(ctx, box, layer-1);
		return;
	}
	children[layer]->draw(ctx, box);

	Bounding_box boxes[4] = { 0 };
	substract(box, children[layer]->get_bounding_box(), boxes);
	for (uint8_t i = 0 ; i < 4 ; i++) {
		if (!boxes[i].is_empty()) {
			draw(ctx, boxes[i], layer-1);
		}
	}
};

void Composite_widget::focus(Widget* widget) {
	focused = widget;
	Widget::focus(this);
}


void Composite_widget::on_mouse_down(int32_t x, int32_t y, uint8_t button) {
	for (int32_t i = num_children - 1 ; i >= 0 ; i--) {
		if (children[i]->contains(x - pos_x, y - pos_y)) {
			children[i]->on_mouse_down(x - pos_x, y - pos_x, button);
			return;
		}
	}
	if (focussable) {
		focus(this);
	}
}
void Composite_widget::on_mouse_up(int32_t x, int32_t y, uint8_t button) {
	for (int32_t i = num_children - 1 ; i >= 0 ; i--) {
		if (children[i]->contains(x - pos_x, y - pos_x)) {
			children[i]->on_mouse_up(x - pos_x, y - pos_x, button);
			break;
		}
	}
}
void Composite_widget::on_mouse_leave(int32_t x, int32_t y) {
	for (int32_t i = num_children - 1 ; i >= 0 ; i--) {
		if (children[i]->contains(x - pos_x, y - pos_x)) {
			children[i]->on_mouse_leave(x - pos_x, y - pos_x);
			break;
		}
	}
}
void Composite_widget::on_mouse_enter(int32_t x, int32_t y) {
	for (int32_t i = num_children - 1 ; i >= 0 ; i--) {
		if (children[i]->contains(x - pos_x, y - pos_x)) {
			children[i]->on_mouse_enter(x - pos_x, y - pos_x);
			break;
		}
	}
}
void Composite_widget::on_mouse_move(int32_t ox, int32_t oy, int32_t nx, int32_t ny) {
	Widget* enter = 0;
	for (int32_t i = num_children - 1 ; i >= 0 ; i--) {
		if (children[i]->contains(nx - pos_x, ny - pos_x)) {
			enter = children[i];
			break;
		}
	}
	Widget* leave = 0;
	for (int32_t i = num_children - 1 ; i >= 0 ; i--) {
		if (children[i]->contains(ox - pos_x, oy - pos_x)) {
			leave = children[i];
			break;
		}
	}
	if (enter == leave && enter != 0) {
		return enter->on_mouse_move(ox - pos_x, oy - pos_x, nx - pos_x, ny - pos_x);
	}
	if (enter != 0) {
		enter->on_mouse_enter(nx - pos_x, ny - pos_x);
	}
	if (leave != 0) {
		leave->on_mouse_leave(ox - pos_x, oy - pos_x);
	}
}

void Composite_widget::on_key_down(char key) {
	if (focused != 0) {
		focused->on_key_down(key);
	}
}
void Composite_widget::on_key_up(char key) {
	if (focused != 0) {
		focused->on_key_up(key);
	}
}
