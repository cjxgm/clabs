
#ifndef __ADJUST_H__
#define __ADJUST_H__

#include "object.h"
#include "button.h"

typedef struct _adjust
{
	OBJECT	obj;
	BUTTON	inc;
	BUTTON	dec;
	int		value;
}
ADJUST;

void adjust_init(ADJUST * adj, float x, float y, float w, float h);
void adjust_draw(ADJUST * adj);
int adjust_mouse(ADJUST * adj, float x, float y, char down);

#endif

