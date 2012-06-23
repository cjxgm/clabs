
/************************************************************
 * cp: font.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include "font.h"
#include "vgafont.h"
#include <string.h>
#include <GL/gl.h>

static void draw_char(float x, float y, unsigned char ch);

void font_render(const char * str, int x, int y)
{
	while (*str) {
		draw_char(x, y, *str++);
		x += 8;
	}
}

int font_width(const char * str)
{
	return strlen(str)*8;
}

void draw_char(float x, float y, unsigned char ch)
{
	int row, col;
	glBegin(GL_POINTS);
	for (row=0; row<16; row++) {
		unsigned char c = font[(ch<<4)|row];
		for (col=0; col<8; col++)
			if ((c>>col)&1)
				glVertex2f(x+8-col, y+16-row);
	}
	glEnd();
}

