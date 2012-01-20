
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "camera.h"

static const float no[] = { 0, 0, 0, 1 };
static char key[256] = {0};
static float fps = 1;

static GLuint lists;
#define LIST_SIZE		1
#define LIST_MAZE		lists

#define W 15
#define H 10
#define MAZE(X,Y)	maze[(((Y) * W) + X)]
const char maze[H*W+1] = 
		"###############"
		"        #     #"
		"####  ### #####"
		"#  # #         "
		"#  #   * ###  #"
		"#  ##### # #  #"
		"#    # # # #  #"
		"# # *  ### #  #"
		"#             #"
		"###############";

void draw_maze(void)
{
	float ground_diffuse[]  = { 0.6, 0.6, 0.6, 1 };
	float wall_diffuse[]    = { 0.8, 0.8, 0.4, 1 };
	float danger_emission[] = { 1.0, 0.6, 0.2, 1 };

	int x, y;
	for (y=0; y<H; y++)
		for (x=0; x<W; x++) {
			glPushMatrix(); {
				glTranslatef(x, 0, y);
				switch (MAZE(x, y)) {
					case '#': // wall
						glMaterialfv(GL_FRONT, GL_DIFFUSE, wall_diffuse);
						glMaterialfv(GL_FRONT, GL_EMISSION, no);
						break;
					case ' ': // ground
						glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_diffuse);
						glMaterialfv(GL_FRONT, GL_EMISSION, no);
						glTranslatef(0, -0.5, 0);
						glScalef(1, 0.1, 1);
						break;
					case '*': // danger
						glMaterialfv(GL_FRONT, GL_DIFFUSE, no);
						glMaterialfv(GL_FRONT, GL_EMISSION,
								danger_emission);
						break;
				}
				glutSolidCube(1.0);
			} glPopMatrix();
		}
}

void key_down(unsigned char k, int x, int y)
{
	key[k] = 1;
}

void key_up(unsigned char k, int x, int y)
{
	key[k] = 0;
}

void glWindowPos2i(int x, int y);

void renderString(int x, int y,
		const char * string, void * font)
{
	if (font == NULL) font = GLUT_BITMAP_HELVETICA_18;
	glWindowPos2i(x, y);
	while (*string)
		glutBitmapCharacter(font, *string++);
}

float vec_dot(float v1[], float v2[])
{
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

int face_side(float v[], float n[], float p[])
{
	float d = vec_dot(n, v) - vec_dot(n, p);
	if (d >= +0.0001) return +1;
	if (d <= -0.0001) return -1;
	return 0;
}

int col_face(float v1[], float v2[], float n[], float p[])
{
	if (face_side(v1, n, p) * face_side(v2, n, p) == -1) return 1;
	return 0;
}

int collide(float v1[], float v2[])
{
	int x, y;
	for (y=0; y<H; y++)
		for (x=0; x<W; x++) {
			if (MAZE(x, y) == '#') {
				float n[3] = {1, 0, 0};
				float p[3] = {x, 0, y};
				if (col_face(v1, v2, n, p)) return 1;
			}
		}
	return 0;
}

void render(void)
{
	unsigned int time = glutGet(GLUT_ELAPSED_TIME);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix(); {
		camApply();
		glCallList(LIST_MAZE);
	} glPopMatrix();

	// draw fps
	char buf[128];
	sprintf(buf, "fps: %.2f", fps);
	glColor3f(0.8, 0, 0);
	renderString(10, 10, buf, NULL);
	// camera matrix
	{
		int y, x;
		for (y=0; y<4; y++)
			for (x=0; x<4; x++) {
				sprintf(buf, "%+.3f", camera[y*4+x]);
				renderString(10 + x * 80, 180 - y*20, buf, NULL);
			}
	}

	float pos[3];
	camGetPosition(pos);
	sprintf(buf, "%+.3f %+.3f %+.3f", pos[0], pos[1], pos[2]);
	renderString(10, 40, buf, NULL);

	glutSwapBuffers();

	if (key[27]) exit(0);

	camBegin();
	if (key['w']) {
		camForward(+4 / fps);
		float pos2[3];
		camGetPosition(pos2);
		if (collide(pos, pos2)) camForward(-4 / fps);
	}
	if (key['s']) {
		camForward(-4 / fps);
		float pos2[3];
		camGetPosition(pos2);
		if (collide(pos, pos2)) camForward(+4 / fps);
	}
	if (key['q']) {
		camPan(+4 / fps);
		float pos2[3];
		camGetPosition(pos2);
		if (collide(pos, pos2)) camPan(-4 / fps);
	}
	if (key['e']) {
		camPan(-4 / fps);
		float pos2[3];
		camGetPosition(pos2);
		if (collide(pos, pos2)) camPan(+4 / fps);
	}
	if (key['d']) camTurn(+90 / fps);
	if (key['a']) camTurn(-90 / fps);
	camEnd();

	fps = 1000.0 / (glutGet(GLUT_ELAPSED_TIME) - time);
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Maze");
	glutIdleFunc(&render);
	glutKeyboardFunc(&key_down);
	glutKeyboardUpFunc(&key_up);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 640.0 / 480.0, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	/* lighting */ {
		float light_pos[] = { 0, 0, 0, 1 };
		float white_light[] = { 1.1, 1.1, 1.1, 1 };
		float ambient[] = { 0.4, 0.4, 0.4, 1 };

		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	lists = glGenLists(LIST_SIZE);
	glNewList(LIST_MAZE, GL_COMPILE);
	draw_maze();
	glEndList();

	camInit();

	glutMainLoop();

	return 0;
}

