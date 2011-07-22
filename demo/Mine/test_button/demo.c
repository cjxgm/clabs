#include "demolino.h"
#include <math.h>
#include "button.c"

#define DEMO_TITLE		"Buttons Demo"
#define DEMO_WIN_SIZE	640, 480

// prototype
void draw_demo_0(void);
void motion(int x, int y);
void mouse(int a, int b, int x, int y);

/*
enum { TEX_ALPHA, TEX_CNT };
GLuint textures[TEX_CNT];
*/
BUTTON btns[4];
ADJUST adjs[4];

/******* main *******/
int main(void)
{
	printf("Buttons Demo\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("    Buttons & Menu\n");
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

void check_error(void)
{
	GLenum err;
	if ((err = glGetError()) == GL_NO_ERROR)
		fprintf(stderr, "Good.\n");
	else
		fprintf(stderr, "\033[01;31mError: \033[0m%s\n",
				gluErrorString(err));
}

void init(void)
{
	// GLubyte img[64*64] = {0};

	glEnable(GL_SCISSOR_TEST);

	int i;
	for (i=0; i<4; i++) {
		button_init(&btns[i], -50, 50 - i*10, 40, 10, "Test");
		adjust_init(&adjs[i], +50, 50 - i*10, 40, 10);
	}
	btns[1].obj.stat = STAT_DISABLED;

	glutPassiveMotionFunc(motion);
	glutMouseFunc(mouse);

	glClearColor(0, 0, 0, 0);
	demo_perspective(true);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	check_error();

	/*
	// Init textures
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glGenTextures(TEX_CNT, textures);

	int x, y;
	for (y=0; y<64; y++)
		for (x=0; x<64; x++) {
			int u = (x-32);
			int v = (y-32);
			int l = sqrt(u*u + v*v) * 0xFF / 32;
			img[y*64 + x] = (l < 0xFF ? 0xFF - l : 0);
			if (x==0 || y==0 || x==63 || y==63)
				img[y*64 + x] = 0xFF;
		}

	glBindTexture(GL_TEXTURE_2D, textures[TEX_ALPHA]);
	glTexImage2D(GL_TEXTURE_2D,
			0,				// Precise level
			GL_ALPHA8,
			64, 64,
			0,				// Border size
			GL_ALPHA, GL_UNSIGNED_BYTE,
			img);
	glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S,
			GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T,
			GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	check_error();
	*/
}

void render(void)
{
	glScissor(0, 0, 640, 480);
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

/*
static float x=-10, y=-10, w=20, h=20;
static int mouse_x = -1, mouse_y = -1;
static int stat = 0;
*/
char is_down = 0;
float ms_x = 0, ms_y = 0;

void proc_event(void)
{
	int i;
	for (i=0; i<4; i++) {
		if (object_mouse(&btns[i].obj, ms_x, ms_y, is_down) == 1)
			exit(0);
		adjust_mouse(&adjs[i], ms_x, ms_y, is_down);
	}
}

void motion(int nx, int ny)
{
	/*
	if (mouse_x == -1) {
		mouse_x = nx;
		mouse_y = ny;
		return;
	}

	if (stat==1) {
		w += (nx - mouse_x) * 200.0 / 480.0;
		h += (ny - mouse_y) * 200.0 / 480.0;
	}
	if (stat==2) {
		x += (nx - mouse_x) * 200.0 / 480.0;
		y += (ny - mouse_y) * 200.0 / 480.0;
	}

	mouse_x = nx;
	mouse_y = ny;
	*/
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
	/*
	if (a==0 && b==1)
		stat=0;
	*/
	if (a == 0)
		is_down = !b;
	
	proc_event();

	/*
	fprintf(stderr, "%d, %d, %d, %d\n", a, b, x, y);
	fflush(stderr);
	*/
}

void draw_demo_0(void)
{
	PUSH; {
		char buf[256];

		glScalef(4, 4, 1);
		sprintf(buf, "%f\n%f", ms_x, ms_y);
		draw_string(buf);
	} POP;

	int i;
	for (i=0; i<4; i++) {
		button_draw(&btns[i]);
		adjust_draw(&adjs[i]);
	}
	
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
		/*
		case 's':
			if (stat==0) stat=1;
		case 'g':
			if (stat==0) stat=2;
		*/
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
