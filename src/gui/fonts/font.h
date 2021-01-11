#ifndef __FONT_H__
#define __FONT_H__

#include "utils/types.h"


struct Font {
	uint8_t* stencils;
	uint8_t width;
	uint8_t height;
	uint8_t size;

	uint8_t* get(char letter);
};


class Fonts {
private:
	static uint8_t FUnknown[];
	static uint8_t Font4x4[];
	static uint8_t Font3x5[];
	static uint8_t Font5x5[];

public:
	Font static get(uint8_t width, uint8_t height);
};


#endif
