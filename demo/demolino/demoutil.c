/** demoutil.c
 * Demolino: Utilities
 **/
#include <math.h>
#include "demoutil.h"
#include "demostr.h"

void draw_circle(float x, float y, float rx, float ry, bool fill)
{
	float a;
	float step = 1 / (rx>ry ? rx : ry);

	glBegin((fill ? GL_POLYGON : GL_LINE_LOOP));
	for (a=0; a<2*PI; a+=step)
		glVertex2f(x + cos(a)*rx, y + sin(a)*ry);
	glEnd();
}

void draw_square(float x1, float y1, float x2, float y2, bool fill)
{
	glBegin((fill ? GL_POLYGON : GL_LINE_LOOP));
	glVertex2f(x1, y1);
	glVertex2f(x1, y2);
	glVertex2f(x2, y2);
	glVertex2f(x2, y1);
	glEnd();
}

void draw_logo(void)
{
	PUSH;
	draw_string2("Clanjor", 1.8);
	glTranslatef(0, -2.5, 0);
	draw_string2("Production", 1.2);
	POP;
}

