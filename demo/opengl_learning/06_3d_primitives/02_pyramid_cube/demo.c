#include "demolino.h"
#include <math.h>

#define DEMO_TITLE		"OpenGL Learning"
#define DEMO_WIN_SIZE	640, 480

// prototype
void draw(void);

float tri_a = 0.0;		// a <=> angle
float tri_s = 4.0;		// s <=> speed
float quad_a = 0.0;
float quad_s = 8.0;
float global_a = 0.0;
float global_s = 2.0;

int main(void)
{
	printf("OpenGL Learning\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("Keys:\n");
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
	demo_perspective(true);
}

void timer(int value)
{
	tri_a += tri_s;
	quad_a += quad_s;
	global_a += global_s;
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
	glRotatef(30, 1, 0, 0);
	glRotatef(-global_a, 1, 0, 0);

	// Draw a pyramid
	PUSH; {
		glTranslatef(-50, 0, 0);
		glRotatef(tri_a, 0, 1, 0);
		glScalef(30, 30, 30);

		glBegin(GL_TRIANGLES); {
			// Front
			glColor3f(1, 0, 0);
			glVertex3f(0, 1, 0);

			glColor3f(0, 1, 0);
			glVertex3f(-1, -1, 1);

			glColor3f(0, 0, 1);
			glVertex3f(1, -1, 1);

			// Right
			glColor3f(1, 0, 0);
			glVertex3f(0, 1, 0);

			glColor3f(0, 0, 1);
			glVertex3f(1, -1, 1);

			glColor3f(0, 1, 0);
			glVertex3f(1, -1, -1);

			// Back
			glColor3f(1, 0, 0);
			glVertex3f(0, 1, 0);

			glColor3f(0, 1, 0);
			glVertex3f(1, -1,-1);

			glColor3f(0, 0, 1);
			glVertex3f(-1, -1, -1);

			// Left
			glColor3f(1, 0, 0);
			glVertex3f(0, 1, 0);

			glColor3f(0, 0, 1);
			glVertex3f(-1, -1, -1);

			glColor3f(0, 1, 0);
			glVertex3f(-1, -1, 1);
		} glEnd();

		// Bottom
		glBegin(GL_QUADS); {
			glColor3f(0, 1, 0);
			glVertex3f(-1, -1, 1);

			glColor3f(0, 0, 1);
			glVertex3f(-1, -1, -1);

			glColor3f(0, 1, 0);
			glVertex3f(1, -1, -1);

			glColor3f(0, 0, 1);
			glVertex3f(1, -1, 1);
		} glEnd();
	} POP;

	// Draw a quad
	PUSH; {
		glTranslatef(50, 0, 0);
		glRotatef(quad_a, 0, 1, 0);
		glScalef(30, 30, 30);

		glBegin(GL_QUADS); {
			// Top
			glColor3f(0.8, 0.6, 0.2);
			glVertex3f(-1, 1, 1);
			glVertex3f(1, 1, 1);
			glVertex3f(1, 1, -1);
			glVertex3f(-1, 1, -1);

			// Bottom
			glColor3f(0.4, 0.3, 0.1);
			glVertex3f(-1, -1, 1);
			glVertex3f(-1, -1, -1);
			glVertex3f(1, -1, -1);
			glVertex3f(1, -1, 1);

			// Front
			glColor3f(0.6, 0.4, 0.2);
			glVertex3f(-1, 1, 1);
			glVertex3f(-1, -1, 1);
			glVertex3f(1, -1, 1);
			glVertex3f(1, 1, 1);

			// Back
			glColor3f(0.6, 0.6, 0.3);
			glVertex3f(-1, 1, -1);
			glVertex3f(1, 1, -1);
			glVertex3f(1, -1, -1);
			glVertex3f(-1, -1, -1);

			// Left
			glColor3f(0.6, 0.4, 0.5);
			glVertex3f(-1, 1, 1);
			glVertex3f(-1, 1, -1);
			glVertex3f(-1, -1, -1);
			glVertex3f(-1, -1, 1);

			// Right
			glColor3f(0.3, 0.7, 0.5);
			glVertex3f(1, 1, 1);
			glVertex3f(1, -1, 1);
			glVertex3f(1, -1, -1);
			glVertex3f(1, 1, -1);
		} glEnd();
	} POP;
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		default:
			fprintf(stderr, "key: %d\n", key);
	}
}

