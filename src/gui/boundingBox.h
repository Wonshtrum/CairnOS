#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include "utils/types.h"
#include "utils/misc.h"


struct Bounding_box {
	int32_t pos_x;
	int32_t pos_y;

	int32_t width;
	int32_t height;

	bool is_empty();
};


bool overlapp(Bounding_box a, Bounding_box b);
Bounding_box intersect(Bounding_box a, Bounding_box b);
void substract(Bounding_box main, Bounding_box sub, Bounding_box boxes[]);


#endif
