#include "demolino.h"
#include "sound.h"
#include <math.h>

#define DEMO_TITLE		"OpenGL Learning"
#define DEMO_WIN_SIZE	640, 480

/*
   Map value in range [SF, ST] to [DS, DT]
MAP:			Linear mapping
MAP_SMOOTH:		Sine mapping, smooth in and smooth out
MAP_SMOOTH_IN:	Sine mapping, smooth in only
MAP_SMOOTH_OUT:	Sine mapping, smooth out only
 */
#define MAP(V,SF,ST,DS,DT)\
	((float)((V) - (SF)) * ((DT) - (DS)) / ((ST) - (SF)) + (DS))
#define MAP_SMOOTH(V,SF,ST,DS,DT)\
	MAP(sin(MAP(V, SF, ST, -PI/2.0, PI/2.0)), -1, 1, DS, DT)
#define MAP_SMOOTH_IN(V,SF,ST,DS,DT)\
	MAP(sin(MAP(V, SF, ST, -PI/2.0, 0)), -1, 0, DS, DT)
#define MAP_SMOOTH_OUT(V,SF,ST,DS,DT)\
	MAP(sin(MAP(V, SF, ST, 0, PI/2.0)), 0, 1, DS, DT)

// prototype
void draw_demo_0(void);

int stage = 0;
int frame = 0;

enum { TEX_ALPHA, TEX_CNT };
GLuint textures[TEX_CNT];

/******* main *******/
int main(void)
{
	printf("OpenGL Learning\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("    Texturing & Blending\n");
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
	GLubyte img[64*64] = {0};

	glClearColor(0, 0, 0, 0);
	demo_perspective(true);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	sound_play();
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
	draw_demo_0();
	POP;

	POP;
	SWAP;
}

typedef struct _particle
{
	float	r;
	float	g;
	float	b;

	int		from, to;		// time-based!

	float	vx;
	float	vy;
	float	ax;
	float	ay;
}
PARTICLE;

PARTICLE particles[100];
#define LEN(A)	(sizeof(A)/sizeof((A)[0]))

void new_particles(void)
{
	int i;
	float l;
	for (i=0; i<LEN(particles); i++) {
		l = MAP(rand()&0xFF, 0, 0xFF, 0.6, 1);
		particles[i].r = 0.8 * l;
		particles[i].g = 0.6 * l;
		particles[i].b = 0;
		particles[i].from = rand()%100;
		particles[i].to = MAP(rand()%200, 0, 200, 200, 1400);
		particles[i].vx = MAP(i, 0, LEN(particles)-1,
								-200,
								200);
		particles[i].vy = MAP(rand()%20, 0, 20, 20, 60);
		particles[i].ax = 0;
		particles[i].ay = MAP(rand()%20, 0, 20, 20, 80);
	}
}

void draw_particles()
{
	float x, y, t;
	int i;

	glBindTexture(GL_TEXTURE_2D, textures[TEX_ALPHA]);
	glEnable(GL_TEXTURE_2D);

	for (i=0; i<LEN(particles); i++) {
		if (time < particles[i].from
			|| time > particles[i].to)
			continue;
		t = (time - particles[i].from) / 1000.0;
		x = (particles[i].vx + particles[i].ax * t / 2) * t;
		y = (particles[i].vy + particles[i].ay * t / 2) * t;

		PUSH;

		glTranslatef(x, y, 0);
		glScalef(20, 20, 1);
		glColor4f(particles[i].r,
				particles[i].g,
				particles[i].b,
				MAP(time, particles[i].from, particles[i].to, 1.0, 0.0));
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
	}

	glDisable(GL_TEXTURE_2D);
}

void draw_demo_0(void)
{
	switch ((sound_get_row()+1)%8){
		case 6:
			time_base += time;
			new_particles();
			break;
	}
	
	glRotatef(MAP(time, 0, 1000, -60, 30), 0, 1, 0);
	PUSH;
	float s = MAP(time, 0, 1000, 0, 6);
	glColor3f(0.8, 0.6, 0.0);
	glScalef(s, s, 1);
	glTranslatef(-11, 0.5, 0);
	draw_string("A Clanjor Production");
	POP;
	draw_particles();
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
