#ifndef __GRAPHICS_CONTEXT_H__
#define __GRAPHICS_CONTEXT_H__

#include "utils/types.h"
#include "gui/color.h"
#include "gui/boundingBox.h"


inline bool pass_stencil(int32_t x, int32_t y, int32_t width, int32_t height, uint8_t* stencil);


class Graphics_context {
protected:
	int32_t width;
	int32_t height;

public:
	Graphics_context(int32_t width, int32_t height);
	~Graphics_context();

	int32_t get_width();
	int32_t get_height();

	virtual void put_pixel(int32_t x, int32_t y, Color color);
	void fill_rectangle(int32_t ox, int32_t oy, int32_t width, int32_t height, Color color);
	void rectangle(int32_t ox, int32_t oy, int32_t width, int32_t height, Color color);
	void stencil(int32_t ox, int32_t oy, int32_t width, int32_t height, uint8_t* stencil, Color color);
	void crop_stencil(int32_t ox, int32_t oy, int32_t width, int32_t height, int32_t crop_x, int32_t crop_y, int32_t crop_width, int32_t crop_height, uint8_t* stencil, Color color);
};


#endif
