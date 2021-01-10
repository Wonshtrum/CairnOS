#include "desktop.h"


Desktop::Desktop(int32_t width, int32_t height, Color color):
	Composite_widget(0, 0, width, height, color),
	Mouse_event_handler(),
	mouse_x(width/2),
	mouse_y(height/2),
	ctx(0) {}
Desktop::~Desktop() {}

Graphics_context* Desktop::get_ctx() {
	return ctx;
}
void Desktop::set_ctx(Graphics_context* ctx) {
	this->ctx = ctx;
}
void Desktop::invalidate(Bounding_box boxes[]) {
	for (uint8_t i = 0 ; i < 4 ; i++) {
		if (!boxes[i].is_empty()) {
			draw(ctx, boxes[i]);
		}
	}
}

void Desktop::focus(Widget* widget) {
	if (widget == this) {
		return;
	}
	add_child(widget);
	Composite_widget::focus(widget);
}

void Desktop::erase_mouse(Graphics_context* ctx, int32_t x, int32_t y) {
	draw(ctx, { x-2, y-2, 5, 5 });
}
void Desktop::draw_mouse(Graphics_context* ctx, int32_t x, int32_t y) {
	for (int32_t i = -2 ; i < 3 ; i++) {
		for (int32_t j = -2 ; j < 3 ; j++) {
			if (i == 0 || j == 0) {
				ctx->put_pixel(x - i, y - j, {0xFF, 0xFF, 0xFF});
			}
		}
	}
}

void Desktop::on_mouse_down(uint8_t button) {
	Composite_widget::on_mouse_down(mouse_x, mouse_y, button);
	draw_mouse(ctx, mouse_x, mouse_y);
}
void Desktop::on_mouse_up(uint8_t button) {
	Composite_widget::on_mouse_up(mouse_x, mouse_y, button);
}
void Desktop::on_mouse_move(int32_t dx, int32_t dy) {
	dx /= 2;
	dy /= 2;
	int32_t old_x = mouse_x;
	int32_t old_y = mouse_y;
	erase_mouse(ctx, mouse_x, mouse_y);
	mouse_x += dx;
	mouse_y += dy;
	if (mouse_x < 0) mouse_x = 0;
	if (mouse_x >= width) mouse_x = width - 1;
	if (mouse_y < 0) mouse_y = 0;
	if (mouse_y >= height) mouse_y = height - 1;
	Composite_widget::on_mouse_move(old_x, old_y, mouse_x, mouse_y);
	draw_mouse(ctx, mouse_x, mouse_y);
}
