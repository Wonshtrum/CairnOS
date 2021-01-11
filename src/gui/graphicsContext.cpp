#include "graphicsContext.h"


bool pass_stencil(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* stencil) {
	uint32_t k = y * width + x;
	return (stencil[k / 8] >> (k % 8)) & 1 == 1;
}


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
void Graphics_context::stencil(int32_t ox, int32_t oy, int32_t width, int32_t height, uint8_t* stencil, Color color) {
	int32_t k;
	for (int32_t x = 0 ; x < width ; x++) {
		for (int32_t y = 0 ; y < height ; y++) {
			if (pass_stencil(x, y, width, height, stencil)) {
				put_pixel(ox + x, oy + y, color);
			}
		}
	}
}
void Graphics_context::crop_stencil(int32_t ox, int32_t oy, int32_t width, int32_t height, int32_t crop_x, int32_t crop_y, int32_t crop_width, int32_t crop_height, uint8_t* stencil, Color color) {
	for (int32_t x = 0 ; x < crop_width ; x++) {
		for (int32_t y = 0 ; y < crop_height ; y++) {
			if (pass_stencil(crop_x - ox + x, crop_y - oy + y, width, height, stencil)) {
				put_pixel(crop_x + x, crop_y + y, color);
			}
		}
	}
}
