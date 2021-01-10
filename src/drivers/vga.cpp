#include "vga.h"
#include "vga_modes.h"


Driver_VGA::Driver_VGA():
	misc_port(0x3C2),
	crtc_index_port(0x3D4),
	crtc_data_port(0x3D5),
	sequence_index_port(0x3C4),
	sequence_data_port(0x3C5),
	graphics_controller_index_port(0x3CE),
	graphics_controller_data_port(0x3CF),
	attribute_controller_index_port(0x3C0),
	attribute_controller_read_port(0x3C1),
	attribute_controller_write_port(0x3C0),
	attribute_controller_reset_port(0x3DA),
	graphics_context(this) {}
Driver_VGA::~Driver_VGA() {}

char* Driver_VGA::get_name() { return "VGA"; }
GC_VGA* Driver_VGA::get_graphics_context() { return &graphics_context; }

void Driver_VGA::write_registers(uint8_t* registers) {
	misc_port.write(*(registers++));
	for (uint8_t i = 0 ; i < VGA_NUM_SEQ_REGS ; i++) {
		sequence_index_port.write(i);
		sequence_data_port.write(*(registers++));
	}
	crtc_index_port.write(0x03);
	crtc_data_port.write(crtc_data_port.read() | 0x80);
	crtc_index_port.write(0x11);
	crtc_data_port.write(crtc_data_port.read() & ~0x80);
	registers[0x03] |= 0x80;
	registers[0x11] &= ~0x80;
	for (uint8_t i = 0 ; i < VGA_NUM_CRTC_REGS ; i++) {
		crtc_index_port.write(i);
		crtc_data_port.write(*(registers++));
	}
	for (uint8_t i = 0 ; i < VGA_NUM_GC_REGS ; i++) {
		graphics_controller_index_port.write(i);
		graphics_controller_data_port.write(*(registers++));
	}
	for (uint8_t i = 0 ; i < VGA_NUM_AC_REGS ; i++) {
		attribute_controller_reset_port.read();
		attribute_controller_index_port.write(i);
		attribute_controller_write_port.write(*(registers++));
	}
	attribute_controller_reset_port.read();
	attribute_controller_index_port.write(0x20);
}

bool Driver_VGA::support_mode(uint32_t width, uint32_t height, uint32_t color_depth) {
	for (uint8_t i = 0 ; i < NUM_G_MODES ; i++) {
		if (g_modes[i].width == width && g_modes[i].height == height && g_modes[i].color_depth == color_depth) {
			return true;
		}
	}
	return false;
}

bool Driver_VGA::set_mode(uint32_t width, uint32_t height, uint32_t color_depth) {
	uint8_t* g_init = 0;
	for (uint8_t i = 0 ; i < NUM_G_MODES ; i++) {
		if (g_modes[i].width == width && g_modes[i].height == height && g_modes[i].color_depth == color_depth) {
			g_init = g_modes[i].g_init;
			break;
		}
	}
	if (g_init == 0) {
		return false;
	}

	this->width = width;
	this->height = width;
	this->color_depth = color_depth;
	
	graphics_context.width = width;
	graphics_context.height = height;

	write_registers(g_init);
	return true;
}

uint8_t* Driver_VGA::get_framebuffer_segment() {
	graphics_controller_index_port.write(0x06);
	uint8_t segment_number = ((graphics_controller_data_port.read() >> 2) & 0x03);
	switch (segment_number) {
		case 0b00: return (uint8_t*)0x00000;
		case 0b01: return (uint8_t*)0xA0000;
		case 0b10: return (uint8_t*)0xB0000;
		case 0b11: return (uint8_t*)0xB8000;
		default: break;
	}
}

void Driver_VGA::put_pixel(int32_t x, int32_t y, uint8_t color_index) {
	if (color_depth == 0) {
		return;
	}
	if (x < 0 || y < 0 || x > width || y > height) {
		return;
	}
	get_framebuffer_segment()[width * y + x] = color_index;
}

uint8_t Driver_VGA::get_color_index(uint8_t r, uint8_t g, uint8_t b) {
	if (r == 0x00 && g == 0x00 && b == 0x00) return 0x00;	// black
	if (r == 0x00 && g == 0x00 && b == 0xA8) return 0x01;	// blue
	if (r == 0x00 && g == 0xA8 && b == 0x00) return 0x02;	// green
	if (r == 0xA8 && g == 0x00 && b == 0x00) return 0x04;	// red
	if (r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F;	// white
	return 0x00;
}

void Driver_VGA::put_pixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b) {
	put_pixel(x, y, get_color_index(r, g, b));
}


GC_VGA::GC_VGA(Driver_VGA* vga): Graphics_context(0, 0), vga(vga) {}
GC_VGA::~GC_VGA() {}

void GC_VGA::put_pixel(int32_t x, int32_t y, Color color) {
	vga->put_pixel(x, y, color.r, color.g, color.b);
}
