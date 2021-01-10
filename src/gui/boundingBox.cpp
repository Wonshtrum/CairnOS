#include "boundingBox.h"


bool Bounding_box::is_empty() {
	return width <= 0 || height <= 0;
}


bool overlapp(Bounding_box a, Bounding_box b) {
	return a.pos_x + a.width > b.pos_x && b.pos_x + b.width > a.pos_x
	&& a.pos_y + a.height > b.pos_y && b.pos_y + b.height > a.pos_y;
}

Bounding_box intersect(Bounding_box a, Bounding_box b) {
	int32_t x1 = max(a.pos_x, b.pos_x);
	int32_t y1 = max(a.pos_y, b.pos_y);
	int32_t x2 = min(a.pos_x + a.width, b.pos_x + b.width);
	int32_t y2 = min(a.pos_y + a.height, b.pos_y + b.height);
	return { x1, y1, x2 - x1, y2 - y1 };
}


Boxes substract(Bounding_box main, Bounding_box sub) {
	Boxes result;
	Bounding_box boxes[4];
	if (!overlapp(main, sub)) {
		result.boxes[0] = main;
		result.n = 1;
		return result;
	}

	// left box
	boxes[0].pos_x = main.pos_x;
	boxes[0].pos_y = main.pos_y;
	boxes[0].width = sub.pos_x - main.pos_x;
	boxes[0].height = main.height;

	// right box
	boxes[1].pos_x = sub.pos_x + sub.width;
	boxes[1].pos_y = main.pos_y;
	boxes[1].width = main.pos_x + main.width - sub.pos_x - sub.width;
	boxes[1].height = main.height;

	// top box
	boxes[2].pos_x = max(main.pos_x, sub.pos_x);
	boxes[2].pos_y = main.pos_y;
	boxes[2].width = min(main.pos_x + main.width, sub.pos_x + sub.width) - boxes[2].pos_x;
	boxes[2].height = sub.pos_y - main.pos_y;

	// bottom box
	boxes[3].pos_x = boxes[2].pos_x;
	boxes[3].pos_y = sub.pos_y + sub.height;
	boxes[3].width = boxes[2].width;
	boxes[3].height = main.pos_y + main.height - sub.pos_y - sub.height;

	result.n = 0;
	for (uint32_t i = 0 ; i < 4 ; i++) {
		if (!boxes[i].is_empty()) {
			result.boxes[result.n++] = boxes[i];
		}
	}
	return result;
}
