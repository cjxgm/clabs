#include "demolino.h"
#include <math.h>

#define DEMO_TITLE		"OpenGL Learning"
#define DEMO_WIN_SIZE	640, 480

#ifndef PI
#define PI 3.1415927f
#endif

float n = 0;
int shading_model = GL_SMOOTH;

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
	printf("    f       Flat shading\n");
	printf("    s       Smooth shading\n");
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

	glShadeModel(shading_model);

	glRotatef(n, 0, 0, 1);
	glScalef(80, 80, 1);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.0f, 0.0f);
	for(i=0; i<=(1<<3); i++) {
		glColor3f(i & (1 << 2), i & (1 << 1), i & (1 << 0));
		glVertex2f(cos(i * PI / 4), sin(i * PI / 4));
	}
	glEnd();
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		case 'a': n += 1; break;
		case 'd': n -= 1; break;
		case 'f': shading_model = GL_FLAT; break;
		case 's': shading_model = GL_SMOOTH; break;
		default:
			fprintf(stderr, "key: %d\n", key);
	}
}

