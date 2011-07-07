#define DEMO_TITLE "Test - Rotating & Orbiting"
#define DEMO_WIN_SIZE 480, 480
#include "demolino.h"
#include <math.h>
#define PI 3.1415927

static float alpha = 0;

// prototype
void draw_pie(float rx, float ry);
void draw(void);

int main(void)
{
	demo_main(DEMO_TITLE, DEMO_WIN_SIZE, 100, 100, 100, 30);
	return 0;
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
}

void timer(int value)
{
	alpha += 3;
	if (alpha > 360) alpha -= 360;
}

void render(void)
{
	CLEAR;
	PUSH;
	draw();
	POP;    
	SWAP;
}

void draw(void)
{
	glRotatef(alpha, 0, 0, 1);
	SET_COLOR_RGB(256, 128, 0);
	draw_pie(10, 10);

	glTranslatef(50, 0, 0);
	glRotatef(alpha*3, 0, 0, 1);
	SET_COLOR_RGB(128, 64, 0);
	draw_pie(6, 6);
}

void draw_pie(float rx, float ry)
{
	float a;
	float step = 1 / (rx>ry ? rx : ry);

	glBegin(GL_POLYGON);
	glVertex2f(0, 0);
	for (a=0.1; a<2*PI-0.1; a+=step)
		glVertex2f(cos(a)*rx, sin(a)*ry);
	glVertex2f(cos(2*PI-0.1)*rx, sin(2*PI-0.1)*ry);
	glEnd();
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
	}
}

