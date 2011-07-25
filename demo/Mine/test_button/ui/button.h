
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "object.h"

typedef struct _button
{
	OBJECT			obj;
	const char	  * label;
}
BUTTON;

void button_init(BUTTON * btn, float x, float y, float w, float h,
				const char * label);
void button_draw(BUTTON * btn);

#endif
