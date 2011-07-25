
#include "object.h"
#include "demolino.h"
#include <stdlib.h>

void object_init(OBJECT * obj, float x, float y, float w, float h)
{
	obj->x = x;
	obj->y = y;
	obj->w = w;
	obj->h = h;
	obj->stat = STAT_NORMAL;
}

int object_mouse(OBJECT * obj, float x, float y, char down)
{
	if (obj->stat == STAT_DISABLED)
		return 0;

	float u = obj->x + obj->w;
	float v = obj->y + obj->h;

	if (x > obj->x && x < u
		&& y > obj->y && y < v) {
		switch (down) {
			case 0:
				down = obj->stat;
				obj->stat = STAT_HOVER;
				if (down == STAT_PRESSED) return 1;
				break;
			case 1:
				down = obj->stat;
				obj->stat = STAT_PRESSED;
				break;
			case 2:
			case 3:
				return down;
		}
	}
	else
		obj->stat = STAT_NORMAL;

	return 0;
}

void object_scissor(OBJECT * obj)
{
	glScissor((obj->x + 100.0 * 4.0 / 3.0) * 480.0 / 200.0,
			(obj->y + 100.0) * 480.0 / 200.0,
			obj->w * 480.0 / 200.0,
			obj->h * 480.0 / 200.0);
}

