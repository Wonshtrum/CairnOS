#include "text.h"


Text::Text(int32_t x, int32_t y, int32_t width, int32_t height, Color color, Font font, bool focussable):
	Widget(x, y, width, height, color, focussable),
	size(0),
	font(font){ transparent = true; }
Text::~Text() {}

void Text::add_text(char* text) {
	while (*text != '\0' && size < MAX_TEXT_SIZE) {
		buffer[size++] = *(text++);
	}
}

int32_t Text::get_cursor() {
	return size;
}
void Text::set_cursor(int32_t cursor) {
	size = cursor;
}

void Text::draw(Graphics_context* ctx, Bounding_box box) {
	box = intersect(box, get_bounding_box());
	int32_t dx = 0, dy = 0;
	screen_offset(dx, dy);
	int32_t x = 0, y = 0;
	int32_t w = font.width;
	int32_t h = font.height;
	for (int32_t i = 0 ; i < size && y < height ; i++, x += w + 1) {
		if (x > width - w) {
			y += h + 1;
			x = 0;
		}
		uint8_t* stencil = font.get(buffer[i]);
		if (stencil != 0) {
			Bounding_box inner = intersect(box, {x + dx, y + dy, w, h});
			ctx->crop_stencil(x + dx, y + dy, w, h, inner.pos_x, inner.pos_y, inner.width, inner.height, stencil, color);
		}
		//ctx->rectangle(x + dx, y + dy, w, h, color);
	}
}
