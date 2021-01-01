#ifndef __VGA_H__
#define __VGA_H__

#include "utils/types.h"
#include "utils/io.h"
#include "hardware/port.h"
#include "drivers/driver.h"

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		1+VGA_NUM_SEQ_REGS+VGA_NUM_CRTC_REGS+VGA_NUM_GC_REGS+VGA_NUM_AC_REGS


class Driver_VGA: public Driver {
protected:
	Port_8bit misc_port;
	Port_8bit crtc_index_port;
	Port_8bit crtc_data_port;
	Port_8bit sequence_index_port;
	Port_8bit sequence_data_port;
	Port_8bit graphics_controller_index_port;
	Port_8bit graphics_controller_data_port;
	Port_8bit attribute_controller_index_port;
	Port_8bit attribute_controller_read_port;
	Port_8bit attribute_controller_write_port;
	Port_8bit attribute_controller_reset_port;

	uint32_t width;
	uint32_t height;
	uint32_t color_depth;

	void write_registers(uint8_t* registers);
	uint8_t* get_framebuffer_segment();

	virtual uint8_t get_color_index(uint8_t r, uint8_t g, uint8_t b);

public:
	Driver_VGA();
	~Driver_VGA();

	virtual char* get_name() override;

	virtual bool set_mode(uint32_t width, uint32_t height, uint32_t color_depth);
	virtual bool support_mode(uint32_t width, uint32_t height, uint32_t color_depth);
	virtual void put_pixel(int32_t x, int32_t y, uint8_t color_index);
	void put_pixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b);
};


#endif
