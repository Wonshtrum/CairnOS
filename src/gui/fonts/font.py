from PIL import Image
from math import ceil
import os
import sys

os.chdir(os.path.dirname(sys.argv[0]))


img = Image.open("fonts.png").convert('L')
fonts = [
	(2, 1, 4, 4, False),
	(77, 5, 4, 4, True),
	(2, 16, 3, 5, True),
	(79, 21, 5, 5, True)]

result = ""
static_def_fonts = ""
static_get = ""
elseif = ""
for x, y, w, h, a in fonts:
	if not a:
		continue
	static_def_fonts += f"\tstatic uint8_t Font{w}x{h}[];\n"
	font = []
	size = ceil(w*h/8)
	for k in range(26):
		c = [0]*size
		for j in range(h):
			for i in range(w):
				if img.getpixel((x+(k%13)*(w+1)+i, y+(k//13)*(h+1)+j)) == 0:
					c[(j*w+i)//8] += 2**((j*w+i)%8)
		font.append(c)
	name = f"Font{w}x{h}"
	result += f"uint8_t Fonts::{name}[] = {{ " + ", ".join(f"0x{h:02x}" for c in font for h in c) + " };\n\n"
	static_get += f"\t{elseif}if (width == {w} && height == {h}) {{ return {{ {name}, width, height, {size} }}; }}\n"
	elseif = "else "

print(result)
print(static_def_fonts)
content_h = """\
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
""" + static_def_fonts + """
public:
	Font static get(uint8_t width, uint8_t height);
};


#endif
"""


content_cpp = """\
#include "font.h"


uint8_t* Font::get(char letter) {
	if ('A' <= letter && letter <= 'Z') {
		return stencils + (letter-'A')*size;
	}
	if ('a' <= letter && letter <= 'z') {
		return stencils + (letter-'a')*size;
	}
	return 0;
}


uint8_t Fonts::FUnknown[256] = { 0xFF };

""" + result + """
Font Fonts::get(uint8_t width, uint8_t height) {
""" + static_get + """
	return { FUnknown, width, height, 0 };
}
"""


with open("font.h", "w") as file_h:
	file_h.write(content_h)

with open("font.cpp", "w") as file_cpp:
	file_cpp.write(content_cpp)
