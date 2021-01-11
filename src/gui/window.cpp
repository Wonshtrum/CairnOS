#include "window.h"


Window::Window(int32_t x, int32_t y, int32_t width, int32_t height, Color color):
	Composite_widget(x, y, width, height, color),
	dragged(false) {}
Window::~Window() {}

void Window::focus(Widget* widget) {
	Composite_widget::focus(widget);
	redraw();
}

void Window::on_mouse_down(int32_t x, int32_t y, uint8_t button) {
	if (!dragged && button == 1) {
		start_drag();
	}
	Composite_widget::on_mouse_down(x, y, button);
}
void Window::on_mouse_up(int32_t x, int32_t y, uint8_t button) {
	if (dragged && button == 1) {
		stop_drag();
	}
	Composite_widget::on_mouse_up(x, y, button);
}
void Window::on_mouse_leave(int32_t x, int32_t y) {
	if (dragged) {
		stop_drag();
	}
	Composite_widget::on_mouse_leave(x, y);
}
void Window::on_mouse_move(int32_t ox, int32_t oy, int32_t nx, int32_t ny) {
	if (dragged) {
		Bounding_box old = get_bounding_box();
		pos_x += nx - ox;
		pos_y += ny - oy;
		Bounding_box boxes[4];
		#if OUTLINE
		old.outline(boxes);
		#else
		substract(old, get_bounding_box(), boxes);
		#endif
		invalidate(boxes);
		redraw();
	}
	Composite_widget::on_mouse_move(ox, oy, nx, ny);
}

void Window::draw(Graphics_context* ctx, Bounding_box box) {
	#if OUTLINE
	if (dragged) {
		Graphics_context* ctx = get_ctx();
		if (ctx != 0) {
			int32_t dx = 0, dy = 0;
			screen_offset(dx, dy);
			ctx->rectangle(dx, dy, width, height, color);
		}
		return;
	}
	#endif
	Composite_widget::draw(ctx, box);
};

void Window::start_drag() {
	dragged = true;
	#if OUTLINE
	transparent = true;
	Bounding_box box[1] = { get_bounding_box() };
	invalidate(box, 1);
	#endif
}

void Window::stop_drag() {
	dragged = false;
	transparent = false;
	redraw();
}
