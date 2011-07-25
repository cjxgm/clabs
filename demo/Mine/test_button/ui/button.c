
#include "button.h"
#include "demolino.h"

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
	glPushMatrix(); {
		glTranslatef(obj->x + ox, obj->y + oy, 0);
		glScalef(3, 3, 0);
		glTranslatef(0.5, 1, 0);
		draw_string(btn->label);
	} glPopMatrix();
}

