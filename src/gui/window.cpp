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
	dragged = button == 1;
	Composite_widget::on_mouse_down(x, y, button);
}
void Window::on_mouse_up(int32_t x, int32_t y, uint8_t button) {
	dragged &= button != 1;
	Composite_widget::on_mouse_up(x, y, button);
}
void Window::on_mouse_leave(int32_t x, int32_t y) {
	dragged = false;
	Composite_widget::on_mouse_leave(x, y);
}
void Window::on_mouse_move(int32_t ox, int32_t oy, int32_t nx, int32_t ny) {
	if (dragged) {
		Bounding_box old = get_bounding_box();
		pos_x += nx - ox;
		pos_y += ny - oy;
		Bounding_box boxes[4];
		substract(old, get_bounding_box(), boxes);
		invalidate(boxes);
		redraw();
	}
	Composite_widget::on_mouse_move(ox, oy, nx, ny);
}

void Window::redraw() {
	Graphics_context* ctx = get_ctx();
	if (ctx != 0) {
		draw(ctx, get_bounding_box());
	}
}
