#include "window.h"


Window::Window(int32_t x, int32_t y, int32_t width, int32_t height, Color color):
	Composite_widget(x, y, width, height, color),
	dragged(false) {}
Window::~Window() {}

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
		pos_x += nx - ox;
		pos_y += ny - oy;
	}
	Composite_widget::on_mouse_move(ox, oy, nx, ny);
}
