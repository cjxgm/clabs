#include "demolino.h"
#include <math.h>

#define DEMO_TITLE		"OpenGL Learning"
#define DEMO_WIN_SIZE	640, 480

#ifndef PI
#define PI 3.1415927f
#endif

int n = 4;

// prototype
void draw(void);

int main(void)
{
	printf("OpenGL Learning\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("Keys:\n");
	printf("    +       Higher resolution\n");
	printf("    -       Lower resolution\n");
	printf("    ESC     Exit\n");
	fflush(stdout);

	demo_main(DEMO_TITLE,		// title
				DEMO_WIN_SIZE,	// w, h
				100,			// view_w
				100,			// view_h
				100,			// view_z
				30);			// fps

	return 0;
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
}

void timer(int value)
{
}

void render(void)
{
	CLEAR;

	// Logo
	glColor3f(0.5, 0.75, 0.25);
	PUSH;
	glTranslatef(70, 90, 0);
	glScalef(5, 5, 1);
	draw_logo();
	POP;

	// Text 'OPENGL LEARNING'
	glColor3f(0.8, 0.55, 0.25);
	PUSH;
	glTranslatef(-120, 90, 0);
	glScalef(6, 6, 1);
	draw_string("OPENGL");
	POP;
	PUSH;
	glTranslatef(-120, 76, 0);
	glScalef(4.2, 4, 1);
	draw_string("LEARNING");
	POP;

	PUSH;
	draw();
	POP;

	SWAP;
}

void draw(void)
{
	int i;

	glColor3f(0.4, 0.55, 0.75);
	glBegin(GL_POINTS);
	for (i=0; i<n; i++) {
		glVertex2f(60 * cos(2 * PI * i / n),
					60 * sin(2 * PI * i / n));
	}
	glEnd();
	glColor3f(0.8, 0.55, 0.25);
	glBegin(GL_POLYGON);
	for (i=0; i<n; i++) {
		glVertex2f(50 * cos(2 * PI * i / n),
					50 * sin(2 * PI * i / n));
	}
	glEnd();
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		case 43: // +
			n++; break;
		case 45: // -
			n--;
			if (n < 3) n = 3;
			break;
		default:
			fprintf(stderr, "key: %d\n", key);
	}
}

