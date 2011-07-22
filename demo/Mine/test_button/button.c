#include <stdlib.h>

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

typedef struct _button
{
	OBJECT			obj;
	const char	  * label;
}
BUTTON;

typedef struct _adjust
{
	OBJECT	obj;
	BUTTON	inc;
	BUTTON	dec;
	int		value;
}
ADJUST;

// prototype
// object
void object_init(OBJECT * obj, float x, float y, float w, float h);
int object_mouse(OBJECT * obj, float x, float y, char down);
void object_scissor(OBJECT * obj);
// button
void button_init(BUTTON * btn, float x, float y, float w, float h,
				const char * label);
void button_draw(BUTTON * btn);
// adjust
void adjust_init(ADJUST * adj, float x, float y, float w, float h);
void adjust_draw(ADJUST * adj);
int adjust_mouse(ADJUST * adj, float x, float y, char down);

// implementation
void button_init(BUTTON * btn, float x, float y, float w, float h,
				const char * label)
{
	object_init(&btn->obj, x, y, w, h);
	btn->label = label;
}

void button_draw(BUTTON * btn)
{
	OBJECT * obj = &btn->obj;
	float u = obj->x + obj->w;
	float v = obj->y + obj->h;
	float ox = 4;
	float oy = 2;

	switch (obj->stat) {
		case STAT_NORMAL:
			glColor3f(0.2, 0.45, 0.0);
			break;
		case STAT_HOVER:
			glColor3f(0.5, 0.75, 0.25);
			ox -= 0.5;
			oy += 0.5;
			break;
		case STAT_PRESSED:
			glColor3f(0.1, 0.35, 0.0);
			ox += 0.5;
			oy -= 0.5;
			break;
		case STAT_DISABLED:
			glColor3f(0.0, 0.25, 0.0);
			break;
	}

	object_scissor(obj);
	glBegin(GL_QUADS); {
		glVertex2f(obj->x, obj->y);
		glVertex2f(u, obj->y);
		glVertex2f(u, v);
		glVertex2f(obj->x, v);
	} glEnd();

	if (obj->stat == STAT_HOVER)
		glColor3f(0, 0, 0);
	else
		glColor3f(0.8, 0.6, 1);
	PUSH; {
		glTranslatef(obj->x + ox, obj->y + oy, 0);
		glScalef(3, 3, 0);
		glTranslatef(0.5, 1, 0);
		draw_string(btn->label);
	} POP;
}

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

