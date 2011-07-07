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

int frame = 0;
int scissor_den = 5;
int scissor_box_w = 160;
int scissor_box_h = 120;

int main(void)
{
	printf("OpenGL Learning\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("Keys:\n");
	printf("    wsadqe  Explore them yourself!\n");
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
	glEnable(GL_SCISSOR_TEST);
}

void timer(int value)
{
	tri_a += tri_s;
	quad_a += quad_s;
	global_a += global_s;

	if (frame == 0)
		glScissor(0, 0, 640, 480);
	else
		glScissor(320 - scissor_box_w / 2,
				  240 - scissor_box_h / 2,
				  scissor_box_w,
				  scissor_box_h);

	frame++;
	frame %= scissor_den;
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
	// Draw the border of scissor box
	glBegin(GL_LINES); {
		float x1 = (-scissor_box_w / 2) * 100 / 240.0;
		float y1 = (-scissor_box_h / 2) * 100 / 240.0;
		float x2 = (scissor_box_w / 2) * 100 / 240.0;
		float y2 = (scissor_box_h / 2) * 100 / 240.0;

		glVertex2f(x1, y1);
		glVertex2f(x1, y2);

		glVertex2f(x1, y2);
		glVertex2f(x2, y2);

		glVertex2f(x2, y2);
		glVertex2f(x2, y1);

		glVertex2f(x2, y1);
		glVertex2f(x1, y1);
	} glEnd();

	glRotatef(30, 1, 0, 0);
	glRotatef(global_a, 0, 1, 0);

	// Draw a triangle
	PUSH; {
		glTranslatef(-50, 0, 0);
		glRotatef(tri_a, 0, 1, 0);
		glScalef(30, 30, 1);

		glBegin(GL_POLYGON); {
			glColor3f(0.8, 0.8, 0.3);
			glVertex2f(0, 0.866);		// 0.866 = sqrt(3) / 2.0

			glColor3f(0.3, 1, 0.6);
			glVertex2f(-1, -0.866);

			glColor3f(0.8, 0.5, 0.4);
			glVertex2f(1, -0.866);
		} glEnd();
	} POP;

	// Draw a quad
	PUSH; {
		glTranslatef(50, 0, 0);
		glRotatef(quad_a, 1, 0, 0);
		glScalef(30, 30, 1);

		glBegin(GL_POLYGON); {
			glColor3f(0.8, 0.8, 0.3);
			glVertex2f(1, 0.866);		// 0.866 = sqrt(3) / 2.0

			glColor3f(0.3, 1, 0.6);
			glVertex2f(-1, 0.866);

			glColor3f(0.8, 0.5, 0.4);
			glVertex2f(-1, -0.866);

			glColor3f(0.3, 0.5, 0.8);
			glVertex2f(1, -0.866);
		} glEnd();
	} POP;
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		case 'w':
			scissor_box_h++;
			break;
		case 's':
			scissor_box_h--;
			break;
		case 'd':
			scissor_box_w++;
			break;
		case 'a':
			scissor_box_w--;
			break;
		case 'e':
			scissor_den++;
			break;
		case 'q':
			scissor_den--;
			if (scissor_den == 0)
				scissor_den = 1;
			break;
		default:
			fprintf(stderr, "key: %d\n", key);
	}
}

