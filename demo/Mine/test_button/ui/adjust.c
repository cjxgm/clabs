
#include "adjust.h"
#include "demolino.h"
#include <stdio.h>

void adjust_init(ADJUST * adj, float x, float y, float w, float h)
{
	object_init(&adj->obj, x, y, w, h);
	button_init(&adj->dec, x+w-10, y, 10, 10, "-");
	button_init(&adj->inc, x+w-10-1-10, y, 10, 10, "+");
}

void adjust_draw(ADJUST * adj)
{
	OBJECT * obj = &adj->obj;
	float u = obj->x + obj->w;
	float v = obj->y + obj->h;

	glColor3f(0.0, 0.25, 0.0);

	object_scissor(obj);
	glBegin(GL_QUADS); {
		glVertex2f(obj->x, obj->y);
		glVertex2f(u, obj->y);
		glVertex2f(u, v);
		glVertex2f(obj->x, v);
	} glEnd();

	glColor3f(0.8, 0.6, 1);
	PUSH; {
		glTranslatef(obj->x + 4, obj->y + 2, 0);
		glScalef(3, 3, 0);
		glTranslatef(0.5, 1, 0);
		char buf[16];
		sprintf(buf, "%d", adj->value);
		draw_string(buf);
	} POP;

	button_draw(&adj->inc);
	button_draw(&adj->dec);
}

int adjust_mouse(ADJUST * adj, float x, float y, char down)
{
	OBJECT * obj = &adj->obj;

	if (obj->stat == STAT_DISABLED)
		return 0;

	float u = obj->x + obj->w;
	float v = obj->y + obj->h;

	if (x > obj->x && x < u
		&& y > obj->y && y < v) {
		if (down == 2)
			adj->value += 10;
		else if (down == 3)
			adj->value -= 10;
	}

	if (object_mouse(&adj->inc.obj, x, y, down) == 1)
		adj->value++;
	if (object_mouse(&adj->dec.obj, x, y, down) == 1)
		adj->value--;

	return 0;
}

