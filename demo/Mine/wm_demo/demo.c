#include "demolino.h"
#include <math.h>

#define DEMO_TITLE		"CjWM Demo"
#define DEMO_WIN_SIZE	640, 480

// prototype
void draw_demo_0(void);
void motion(int x, int y);
void mouse(int a, int b, int x, int y);

enum { TEX_ALPHA, TEX_CNT };
GLuint textures[TEX_CNT];

/******* main *******/
int main(void)
{
	printf("CjWM Demo\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("    Texturing & Blending\n");
	printf("Keys:\n");
	printf("    ESC     Exit\n");
	printf("    g       grab (move)\n");
	printf("    s       scale (resize)\n");
	printf("    Left Click    Confirm\n");
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
	GLubyte img[64*64] = {0};

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

static float x=-10, y=-10, w=20, h=20;
static int mouse_x = -1, mouse_y = -1;
static int stat = 0;

void motion(int nx, int ny)
{
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
}

void mouse(int a, int b, int x, int y)
{
	if (a==0 && b==1)
		stat=0;
	fprintf(stderr, "%d, %d, %d, %d\n", a, b, x, y);
}

void draw_particles()
{
	glBindTexture(GL_TEXTURE_2D, textures[TEX_ALPHA]);
	glEnable(GL_TEXTURE_2D);

		PUSH;

		glTranslatef(x, -y, 0);
		glScalef(w, -h, 1);
		glTranslatef(0.5, 0.5, 0);
		glColor3f(0.8, 0.6, 0);
		glBegin(GL_QUADS); {
			glTexCoord2f(0, 0);
			glVertex2f(-0.5, -0.5);
			glTexCoord2f(1, 0);
			glVertex2f(0.5, -0.5);
			glTexCoord2f(1, 1);
			glVertex2f(0.5, 0.5);
			glTexCoord2f(0, 1);
			glVertex2f(-0.5, 0.5);
		} glEnd();
		POP;

	glDisable(GL_TEXTURE_2D);
}

void draw_demo_0(void)
{
	draw_particles();
}


void key_press(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		case 's':
			if (stat==0) stat=1;
		case 'g':
			if (stat==0) stat=2;
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
