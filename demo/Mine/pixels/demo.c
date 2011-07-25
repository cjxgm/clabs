#include "demolino.h"
#include <math.h>
#include "pixmap.h"

#define DEMO_TITLE		"Pixels"
#define DEMO_WIN_SIZE	640, 480

PIXMAP * pm = NULL;
int prec = 10;

// prototype
void draw_demo_0(void);
void motion(int x, int y);
void mouse(int a, int b, int x, int y);

/******* main *******/
int main(void)
{
	printf("Pixels\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("Keys:\n");
	printf("    ESC     Exit\n");
	fflush(stdout);

	demo_main(DEMO_TITLE,		// title
			DEMO_WIN_SIZE,		// w, h
			100,				// view_w
			100,				// view_h
			100,				// view_z
			30);				// fps // DO NOT NEED THIS ANY MORE!

	return 0;
}

void newpm(void)
{
	int x, y;

	if (pm) pixmap_free(pm);

	pm = pixmap_new(prec, prec);
	for (y=0; y<pm->h; y++)
		for (x=0; x<pm->w; x++) {
			pixmap_put(pm, x, y, PIXMAP_RGB(x*255/(pm->w-1),
											y*255/(pm->h-1),
											(x+y)*255/(pm->w+pm->h-1)));
		}

}

void check_error(void)
{
	GLenum err;
	if ((err = glGetError()) == GL_NO_ERROR)
		fprintf(stderr, "\e[01mGood.\e[m\n");
	else
		fprintf(stderr, "\e[01;31mError: \e[m%s\n",
				gluErrorString(err));
}

void init(void)
{
	newpm();

	glutPassiveMotionFunc(motion);
	glutMouseFunc(mouse);

	glClearColor(0, 0, 0, 0);
	demo_perspective(true);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	check_error();
}

void render(void)
{
	CLEAR;
	PUSH;

	// Logo
	glColor3f(0.5, 0.75, 0.25);
	PUSH;
	glTranslatef(70, 90, 0);
	glScalef(5, 5, 1);
	draw_logo();
	POP;

	PUSH;
	draw_demo_0();
	POP;

	POP;
	SWAP;
}

char is_down = 0;
float ms_x = 0, ms_y = 0;

void proc_event(void)
{
}

void motion(int nx, int ny)
{
	ms_x = nx * 200.0 / 480.0 - 100.0 * 4.0 / 3.0;
	ms_y = 100 - ny * 200.0 / 480.0;

	/*
	fprintf(stderr, "%f, %f\n", ms_x, ms_y);
	fflush(stderr);
	*/

	proc_event();
}

void mouse(int a, int b, int x, int y)
{
	if (a == 0)
		is_down = 1;
	if (a == 3 || a == 4) // Scroll up/down
		is_down = a - 1;

	if (b == 1)
		is_down = 0;

	ms_x = x * 200.0 / 480.0 - 100.0 * 4.0 / 3.0;
	ms_y = 100 - y * 200.0 / 480.0;
	
	proc_event();
}

void draw_demo_0(void)
{
	prec = MAP_SMOOTH_IN(time%6000, 0, 6000, 2, 100);
	newpm();
	pixmap_draw_full(pm);


	PUSH; {
		char buf[256];

		glScalef(4, 4, 1);
		sprintf(buf, "%f\n%f", ms_x, ms_y);
		draw_string(buf);
	} POP;


	/* GRID
	glBegin(GL_LINES);
	for (i=-100; i<=100; i+=10) {
		glVertex2f(i, 100);
		glVertex2f(i, -100);
		glVertex2f(-100, i);
		glVertex2f(100, i);
	}
	glEnd();
	*/
}

void key_press(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		default:
			fprintf(stderr, "key: %d\n", key);
	}
}

void special_press(int key, int x, int y)
{
	switch (key){
		default:
			fprintf(stderr, "special: %d\n", key);
	}
}
