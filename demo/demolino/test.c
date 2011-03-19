#define DEMO_TITLE "Test"
#define DEMO_WIN_SIZE 480, 480
#include "demolino.h"
#include <math.h>
#define PI 3.1415927

float spacing = 0;
// prototype
void draw(void);

int main(void)
{
	demo_main(DEMO_TITLE, DEMO_WIN_SIZE, 30);
	return 0;
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
}

void timer(int value)
{
	spacing += 0.1;
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
	PUSH;
	glTranslatef(-40, 0, 0);
	glScalef(4.5, 4.5, 1);
	glRotatef(90-spacing*180/PI, 1, 0, 1);
	draw_string3("Welcome\nWelcome\nWelcome",
					sin(spacing)+2,
					(sin(spacing)+1)*2);
	POP;

	PUSH;
	glTranslatef(-80, 80, 0);
	glScalef(5, 5, 1);
	draw_string2("Clanjor", 1.8);
	glTranslatef(0, -2.5, 0);
	draw_string2("Production", 1.2);
	POP;
}

