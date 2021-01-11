#ifndef __TEXT_H__
#define __TEXT_H__

#include "utils/types.h"
#include "gui/config.h"
#include "gui/color.h"
#include "gui/graphicsContext.h"
#include "gui/boundingBox.h"
#include "gui/widget.h"
#include "gui/fonts/font.h"


class Text: public Widget {
private:
	char* buffer;
	int32_t size;
	
	Font font;

public:
	Text(int32_t x, int32_t y, int32_t width, int32_t height, Color color, Font font, bool focussable = true);
	~Text();

	void add_text(char* text);
	int32_t get_cursor();
	void set_cursor(int32_t cursor);

	virtual void draw(Graphics_context* ctx, Bounding_box box) override;
};


#endif
