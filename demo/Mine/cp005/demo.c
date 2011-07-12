#include "demolino.h"
#include "sound.h"
#include <math.h>

#define DEMO_TITLE		"A Clanjor Production - cp005"
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
void draw_cube(void);

void draw_demo_0(void);
void draw_demo_1(void);
void draw_demo_2(void);

int stage = 0;
int row = 0;
int fps = 0;
int frame = 0;
int frame_time = 0;
int pause_time = 0;

typedef struct _cube
{
	float x;
	float y;
	float z;

	float rotx;
	float roty;
	float rotz;
}
CUBE;

#define W			10
#define H			10
#define FRAMES		4
CUBE cube_frame[W*H][FRAMES];

/******* main *******/
int main(void)
{
	printf("A Clanjor Production (No. 005)\n");
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

void init(void)
{
	glClearColor(0, 0, 0, 0);
	demo_perspective(true);
}

void render(void)
{
	CLEAR;
	PUSH;

	switch (stage) {
		case 0:
			switch (frame) {
				case 0:
					{
						int y, x;
						for (y=0; y<H; y++)
							for (x=0; x<W; x++) {
								cube_frame[y * W + x][0] = (CUBE)
								{
									.x = (x - 5) * 20,
										.y = 0,
										.z = (y - 5) * 20,

										.rotx = 0,
										.roty = 0,
										.rotz = 0,
								};
								cube_frame[y * W + x][1] = (CUBE)
								{
									.x = 0,
										.y = 0,
										.z = 0,

										.rotx = 0,
										.roty = 0,
										.rotz = 0,
								};
								cube_frame[y * W + x][2] = (CUBE)
								{
									.x = (x - 5) * 20,
										.y = (rand() % 200),
										.z = (y - 5) * 20,

										.rotx = (rand() % 180) - 90,
										.roty = (rand() % 180) - 90,
										.rotz = (rand() % 180) - 90,
								};
								cube_frame[y * W + x][3] = (CUBE)
								{
									.x = (5 - x) * 20,
										.y = 0,
										.z = (5 - y) * 20,

										.rotx = 0,
										.roty = 0,
										.rotz = 0,
								};
							}
					}
					break;
				default:
					time_base += time;
					stage++;
			}
			frame++;
		case 1:
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
			if (time >= 4000) stage++;
			break;
		case 2:
			glColor3f(MAP_SMOOTH(time%1000, 0, 1000, 1.0, 0.0),
					MAP_SMOOTH(time%1000, 0, 1000, 1.0, 0.0),
					MAP_SMOOTH(time%1000, 0, 1000, 0.5, 0.0));
			glScalef(6, 6, 1);
			glTranslatef(-5, 0, 0);
			draw_string("Loading...");
			if (time >= 5000) {
				stage++;
				sound_play();
				time_base += time;
			}
			break;
		case 3:
			draw_demo_0();
			break;
		case 4:
			// Logo
			glColor3f(0.5, 0.75, 0.25);
			PUSH;
			glTranslatef(70, 90, 0);
			glScalef(5, 5, 1);
			draw_logo();
			POP;

			// Row and fps
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
			draw_demo_1();
			break;
		case 5:
			draw_demo_2();
			break;
		case 6:
			exit(0);
			break;
	}

	POP;
	SWAP;
}

void draw_cube(void)
{
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
}

void draw_demo_0(void)
{
	if (time < 4000) {
		// Camera
		glTranslatef(MAP(time, 0, 4000, 0, -16), 0,
				MAP(time, 0, 4000, 0, 220));

		if (time < 2000)
			glRotatef(MAP_SMOOTH_IN(time, 0, 2000, 0, -50), 0, 1, 0);
		else if (time < 4000)
			glRotatef(MAP(time, 2000, 4000, -50, -120), 0, 1, 0);

		if (time < 2000) {
			PUSH; {
				if (time < 1000)
					glColor3f(1.0, 1.0, 0.5);
				else if (time < 2000)
					glColor3f(MAP_SMOOTH_IN(time, 1000, 2000, 1.0, 0),
							MAP_SMOOTH_IN(time, 1000, 2000, 1.0, 0),
							MAP_SMOOTH_IN(time, 1000, 2000, 0.5, 0));
				glScalef(6, 6, 1);
				glTranslatef(-10, 0, 0);
				draw_string("A Clanjor Production");
			} POP;
		}

		PUSH; {
			if (time < 1000)
				glColor3f(MAP_SMOOTH_IN(time, 0, 1000, 0, 0.2),
						MAP_SMOOTH_IN(time, 0, 1000, 0, 0.6),
						MAP_SMOOTH_IN(time, 0, 1000, 0, 1.0));
			else if (time >= 3800 && time < 4000)
				glColor3f(MAP(time, 3800, 4000, 0.2, 0),
						MAP(time, 3800, 4000, 0.6, 0),
						MAP(time, 3800, 4000, 1.0, 0));
			else
				glColor3f(0.2, 0.6, 1.0);
			glTranslatef(-62, 0, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(6, 6, 1);
			glTranslatef(-3, 0, 0);
			draw_string("cp 005");
		} POP;
	}
	else if (time >= 5000) {
		stage++;
		time_base += time;
	}
}

void draw_demo_1(void)
{
	// Camera
	int t = time % 6000;
	switch (t/2000) {
		case 0:
			glTranslatef(0, 0, MAP_SMOOTH_IN(t, 0, 2000, 0, -120));
			glRotatef(MAP_SMOOTH_IN(t, 0, 2000, 40, 20), 1, 0, 0);
			break;
		case 1:
			glTranslatef(0, 0, -120);
			if (t < 3000)
				glRotatef(MAP(t, 2000, 3000, 20, 10), 1, 0, 0);
			else
				glRotatef(MAP(t, 3000, 4000, 10, 20), 1, 0, 0);
			break;
		case 2:
			glTranslatef(0, 0, MAP_SMOOTH_OUT(t, 4000, 6000, -120, 0));
			glRotatef(MAP_SMOOTH_OUT(t, 4000, 6000, 20, 40), 1, 0, 0);
			break;
	}
	glRotatef(MAP(t, 0, 6000, 0, 360), 0, 1, 0);

	// Draw ground
	PUSH; {
		int i;

		glColor3f(0.8, 0.55, 0.25);
		glBegin(GL_LINES); {
			for (i=-100; i<=100; i+=10) {
				glVertex3f(i, 0, -100);
				glVertex3f(i, 0,  100);
				glVertex3f(-100, 0, i);
				glVertex3f( 100, 0, i);
			}
		} glEnd();
	} POP;

	t = time % (1000 + 2000 * FRAMES);
	PUSH; {
		int i;
		int tmp;
		int tmp2;

		for (i=0; i<W*H; i++) {
			tmp = (t - 1000) / 2000;
			tmp2 = (tmp + 1) % FRAMES;

			PUSH;
			if (t < 1000)
				glTranslatef(cube_frame[i][0].x,
						cube_frame[i][0].y,
						cube_frame[i][0].z);
			else
				glTranslatef(MAP_SMOOTH(t, 1000 + tmp * 2000,
							1000 + (tmp + 1) * 2000,
							cube_frame[i][tmp].x,
							cube_frame[i][tmp2].x),
						MAP_SMOOTH(t, 1000 + tmp * 2000,
							1000 + (tmp + 1) * 2000,
							cube_frame[i][tmp].y,
							cube_frame[i][tmp2].y),
						MAP_SMOOTH(t, 1000 + tmp * 2000,
							1000 + (tmp + 1) * 2000,
							cube_frame[i][tmp].z,
							cube_frame[i][tmp2].z)
						);
			glTranslatef(-8, -0.5, -8);
			if (t < 1000) {}
			else {
				glRotatef(MAP_SMOOTH(t, 1000 + tmp * 2000,
							1000 + (tmp + 1) * 2000,
							cube_frame[i][tmp].rotx,
							cube_frame[i][tmp2].rotx),
						1, 0, 0);
				glRotatef(MAP_SMOOTH(t, 1000 + tmp * 2000,
							1000 + (tmp + 1) * 2000,
							cube_frame[i][tmp].roty,
							cube_frame[i][tmp2].roty),
						0, 1, 0);
				glRotatef(MAP_SMOOTH(t, 1000 + tmp * 2000,
							1000 + (tmp + 1) * 2000,
							cube_frame[i][tmp].rotz,
							cube_frame[i][tmp2].rotz),
						0, 0, 1);
			}
			glTranslatef(8, 0.5, 8);
			glScalef(8, 0.5, 8);
			draw_cube();
			POP;
		}
	} POP;
	if (time >= (1000 + 2000 * FRAMES) * 2) {
		stage++;
		time_base += time;
	}
}

void draw_demo_2(void)
{
	if (time < 4000) {
		// Camera
		glTranslatef(0, 0, -20);
		glTranslatef(MAP(time, 0, 4000, 0, -46), 0,
				MAP(time, 0, 4000, 0, 220));

		if (time < 2000)
			glRotatef(MAP_SMOOTH_IN(time, 0, 2000, 0, -50), 0, 1, 0);
		else if (time < 4000)
			glRotatef(MAP(time, 2000, 4000, -50, -120), 0, 1, 0);

		if (time < 2000) {
			PUSH; {
				if (time < 1000)
					glColor3f(1.0, 1.0, 0.5);
				else if (time < 2000)
					glColor3f(MAP_SMOOTH_IN(time, 1000, 2000, 1.0, 0),
							MAP_SMOOTH_IN(time, 1000, 2000, 1.0, 0),
							MAP_SMOOTH_IN(time, 1000, 2000, 0.5, 0));
				glScalef(6, 6, 1);
				glTranslatef(-10, 0, 0);
				draw_string("Everything By");
			} POP;
		}

		PUSH; {
			if (time < 1000)
				glColor3f(MAP_SMOOTH_IN(time, 0, 1000, 0, 0.2),
						MAP_SMOOTH_IN(time, 0, 1000, 0, 0.6),
						MAP_SMOOTH_IN(time, 0, 1000, 0, 1.0));
			else if (time >= 3800 && time < 4000)
				glColor3f(MAP(time, 3800, 4000, 0.2, 0),
						MAP(time, 3800, 4000, 0.6, 0),
						MAP(time, 3800, 4000, 1.0, 0));
			else
				glColor3f(0.2, 0.6, 1.0);
			glTranslatef(-62, 0, 0);
			glRotatef(90, 0, 1, 0);
			glScalef(6, 6, 1);
			glTranslatef(-4, 0, 0);
			draw_string("eXerigumo Clanjor");
		} POP;
	}
	else if (time >= 5000) {
		stage++;
		time_base += time;
	}
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
