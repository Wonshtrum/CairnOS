#include "graphicsContext.h"


Graphics_context::Graphics_context(int32_t width, int32_t height): width(width), height(height) {}
Graphics_context::~Graphics_context() {}

int32_t Graphics_context::get_width() { return width; }
int32_t Graphics_context::get_height() { return height; }

void Graphics_context::put_pixel(int32_t x, int32_t y, Color color) {}
void Graphics_context::fill_rectangle(int32_t ox, int32_t oy, int32_t width, int32_t height, Color color) {
	for (int32_t x = 0 ; x < width ; x++) {
		for (int32_t y = 0 ; y < height ; y++) {
			put_pixel(ox + x, oy + y, color);
		}
	}
}
void Graphics_context::rectangle(int32_t ox, int32_t oy, int32_t width, int32_t height, Color color) {
	for (int32_t x = 0 ; x < width ; x++) {
		put_pixel(ox + x, oy, color);
		put_pixel(ox + x, oy + height - 1, color);
	}
	for (int32_t y = 0 ; y < height ; y++) {
		put_pixel(ox, oy + y, color);
		put_pixel(ox + width - 1, oy + y, color);
	}
}
