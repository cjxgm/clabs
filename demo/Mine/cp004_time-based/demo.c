#include "demolino.h"
#include "sound.h"
#include <math.h>

#define DEMO_TITLE		"A Clanjor Production - cp004"
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
void draw(void);

int stage = 0;
int row = 0;
int fps = 0;
int frame = 0;
int frame_time = 0;

/******* main *******/
int main(void)
{
	printf("A Clanjor Production (No. 004)\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("Keys:\n");
	printf("    ESC     Exit\n");
	fflush(stdout);

	demo_main(DEMO_TITLE,		// title
			  DEMO_WIN_SIZE,	// w, h
			  100,				// view_w
			  100,				// view_h
			  100,				// view_z
			  30);				// fps

	return 0;
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
	demo_perspective(true);
}

void render(void)
{
	CLEAR;

	switch (stage) {
		case 0:
			time = 0;
			stage++;
		case 1:
			PUSH;
			if ((time / 1000) & 1)
				glColor3f(MAP_SMOOTH(time%1000, 0, 1000, 0.2, 1.0),
						  MAP_SMOOTH(time%1000, 0, 1000, 0.6, 1.0),
						  MAP_SMOOTH(time%1000, 0, 1000, 1.0, 0.5));
			else
				glColor3f(MAP_SMOOTH(time%1000, 0, 1000, 1.0, 0.2),
						  MAP_SMOOTH(time%1000, 0, 1000, 1.0, 0.6),
						  MAP_SMOOTH(time%1000, 0, 1000, 0.5, 1.0));
			glScalef(6, 6, 1);
			glTranslatef(-5, 0, 0);
			draw_string("Loading...");
			POP;
			if (time >= 4000) stage++;
			break;
		case 2:
			PUSH;
			glColor3f(MAP_SMOOTH(time%1000, 0, 1000, 1.0, 0.0),
					  MAP_SMOOTH(time%1000, 0, 1000, 1.0, 0.0),
					  MAP_SMOOTH(time%1000, 0, 1000, 0.5, 0.0));
			glScalef(6, 6, 1);
			glTranslatef(-5, 0, 0);
			draw_string("Loading...");
			POP;
			if (time >= 5000) {
				stage++;
				sound_play();
				time = 0;
			}
			break;
		case 3:
			// Logo
			glColor3f(0.5, 0.75, 0.25);
			PUSH;
			glTranslatef(70, 90, 0);
			glScalef(5, 5, 1);
			draw_logo();
			POP;

			// Row
			PUSH; {
				char buf[40];

				if (time % 1000 > frame_time)
					frame_time = time % 1000;
				else {
					fps = frame;
					frame = 0;
					frame_time = 0;
				}
				frame++;

				glTranslatef(-120, -90, 0);
				glScalef(5, 5, 1);
				sprintf(buf, "row %2.2X\tfps %d", row, fps);
				draw_string(buf);

			} POP;

			// Draw
			PUSH;
			draw();
			POP;
			break;
	}

	SWAP;
}

void draw(void)
{
	// Camera
	int t = time % 6000;
	switch (t/2000) {
		case 0:
			glTranslatef(0, 0, MAP_SMOOTH_IN(t, 0, 2000, 0, -120));
			glRotatef(MAP_SMOOTH_IN(t, 0, 2000, 30, 0), 1, 0, 0);
			glRotatef(MAP_SMOOTH_IN(t, 0, 2000, 0, 120), 0, 1, 0);
			break;
		case 1:
			glTranslatef(0, 0, -120);
			if (t < 3000)
				glRotatef(MAP(t, 2000, 3000, 0, -10), 1, 0, 0);
			else
				glRotatef(MAP(t, 3000, 4000, -10, 0), 1, 0, 0);
			glRotatef(MAP(t, 2000, 4000, 120, 240), 0, 1, 0);
			break;
		case 2:
			glTranslatef(0, 0, MAP_SMOOTH_OUT(t, 4000, 6000, -120, 0));
			glRotatef(MAP_SMOOTH_OUT(t, 4000, 6000, 0, 30), 1, 0, 0);
			glRotatef(MAP_SMOOTH_OUT(t, 4000, 6000, 240, 360), 0, 1, 0);
			break;
	}

	// Draw ground
	PUSH; {
		int i;

		glColor3f(0.8, 0.55, 0.25);
		glTranslatef(0, -30, 0);
		glBegin(GL_LINES); {
			for (i=-100; i<=100; i+=10) {
				glVertex3f(i, 0, -100);
				glVertex3f(i, 0,  100);
				glVertex3f(-100, 0, i);
				glVertex3f( 100, 0, i);
			}
		} glEnd();
	} POP;

	// Sound FX
	row = sound_get_row();
	switch (row % 0x20) {
		case 0x04:
		case 0x0C:
		case 0x14:
		case 0x18:
		case 0x1A:
		case 0x1C:
			glScalef(1.2, 1.2, 1.2);
	}

	// Draw a pyramid
	PUSH; {
		glTranslatef(-50, 0, 0);
		glRotatef(MAP(time%2000, 0, 2000, 0, 360), 0, 1, 0);
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

	// Draw a cube
	PUSH; {
		glTranslatef(50, 0, 0);
		glRotatef(MAP_SMOOTH(time%4000, 0, 4000, 0, 360*2), 0, 1, 0);
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
