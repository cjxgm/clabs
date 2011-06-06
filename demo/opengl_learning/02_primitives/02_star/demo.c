#include "demolino.h"
#include <math.h>

#define DEMO_TITLE		"OpenGL Learning"
#define DEMO_WIN_SIZE	640, 480

#ifndef PI
#define PI 3.1415927f
#endif

float n = 0;
float points[5][2];

// prototype
void draw(void);

int main(void)
{
	printf("OpenGL Learning\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("Keys:\n");
	printf("    a       Rotate Left\n");
	printf("    d       Rotate Right\n");
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

	// Calculate points
	float a = 1 / (2 - 2 * cos(72 * PI / 180));
	float bx = a * cos(18 * PI / 180);
	float by = a * sin(18 * PI / 180);
	float cy = -a * cos(18 * PI / 180);

	points[0][0] = 0;
	points[0][1] = a;

	points[1][0] = bx;
	points[1][1] = by;

	points[2][0] = 0.5;
	points[2][1] = cy;

	points[3][0] = -0.5;
	points[3][1] = cy;

	points[4][0] = -bx;
	points[4][1] = by;
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
	glScalef(100, 100, 1);
	glRotatef(n, 0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	for (i=0; i<sizeof(points)/sizeof(points[0]); i++)
		glVertex2fv(points[i]);
	glEnd();
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		case 'a': // a
			n += 1; break;
		case 'd': // d
			n -= 1; break;
		default:
			fprintf(stderr, "key: %d\n", key);
	}
}

