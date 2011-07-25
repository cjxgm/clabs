
#ifndef __OBJECT_H__
#define __OBJECT_H__

enum { STAT_NORMAL, STAT_HOVER, STAT_PRESSED, STAT_DISABLED };

typedef struct _object
{
	int		stat;
	float	x;
	float	y;
	float	w;
	float	h;
}
OBJECT;

void object_init(OBJECT * obj, float x, float y, float w, float h);
int object_mouse(OBJECT * obj, float x, float y, char down);
void object_scissor(OBJECT * obj);

#endif

