#include "textModeMouse.h"


Text_mode_mouse::Text_mode_mouse(int32_t width, int32_t height): width(width), height(height) {}
Text_mode_mouse::~Text_mode_mouse() {}

void Text_mode_mouse::blink() {
	static uint16_t* video_mem = (uint16_t*)0xb8000;
	video_mem[width*y+x] = 0
		| ((video_mem[width*y+x] & 0xF000) >> 4)
		| ((video_mem[width*y+x] & 0x0F00) << 4)
		| ((video_mem[width*y+x] & 0x00FF));
}

void Text_mode_mouse::on_activate() {
	x = width / 2;
	y = height / 2;
	blink();
}

void Text_mode_mouse::on_deactivate() {
	blink();
}

void Text_mode_mouse::on_mouse_move(int32_t dx, int32_t dy) {
	blink();

	x += dx;
	y += dy;
	if (x < 0) x = 0;
	if (x >= width) x = width - 1;
	if (y < 0) y = 0;
	if (y >= height) y = height - 1;

	blink();
}

static char* labels[] = {"1", "2", "3"};
void Text_mode_mouse::on_mouse_down(uint8_t button) {
	print_str("B_DOWN ");
	print_str(labels[button - 1]);
	print_str("\n");
}

void Text_mode_mouse::on_mouse_up(uint8_t button) {
	print_str("B_UP ");
	print_str(labels[button - 1]);
	print_str("\n");
}
