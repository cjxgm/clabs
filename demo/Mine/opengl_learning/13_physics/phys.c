
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "camera.h"
#include "vector.h"

#define DOTS_CNT	10

static char key[256] = {0};
static float dots[DOTS_CNT][3];
static float dotso[DOTS_CNT][3];
static float fps = 1;
static float spf = 0.00;
static float g = -0.5;

#define PSIZE	10
static float plane_vs[4][3] = {
	{PSIZE, -1, PSIZE},		// 0
	{PSIZE, 10, -PSIZE},		// 1
	{-PSIZE, -1, -PSIZE},		// 2
	{-PSIZE, -1, PSIZE},		// 3
};
static int plane_fs[2][3] = {
	{0, 1, 2},
	{0, 2, 3},
};

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

float classify(float n[3], float p[3], float s[3])
{
	return vec_dot(n, s) - vec_dot(n, p);
}

int colp(float n[3], float p[3],
		float s1[3], float s2[3], float cp[3])
{
	float c[2] = {
		classify(n, p, s1),
		classify(n, p, s2),
	};

	if (c[0]*c[1] >= 0) return 0;

	float d[3];
	vec_sub(d, s2, s1);
	vec_normv(d);
	vec_mulv(d, fabs(c[0]));
	vec_add(cp, s1, d);

	return 1;
}

int coltri(float t1[3], float t2[3], float t3[3],
			float s1[3], float s2[3], float cp[3])
{
	float e[3][3], n[3];
	vec_sub(e[0], t2, t1);
	vec_sub(e[1], t3, t2);
	vec_unit_normal(n, e[0], e[1]);
	if (!colp(n, t1, s1, s2, cp))
		return 0;
	
	return 1;
}

void col(float pos[3], float poso[3])
{
	float cp[3];
	int i;
	for (i=0; i<2; i++) {
		if (coltri(plane_vs[plane_fs[i][0]],
				   plane_vs[plane_fs[i][1]],
				   plane_vs[plane_fs[i][2]],
				   pos, poso, cp)) {
			//vec_sub(poso, pos, poso);
			//vec_mulv(poso, 0.2);
			//vec_addv(poso, pos);
			//vec_cpy(pos, cp);
			vec_cpy(pos, poso);
		}
	}
}

void move(float pos[3], float poso[3])
{
	float t[3];
	vec_cpy(t, pos);
	vec_addv(pos, pos);
	vec_subv(pos, poso);
	pos[1] += g*spf*spf;
	vec_cpy(poso, t);
	col(pos, poso);
}

void render(void)
{
	unsigned int time = glutGet(GLUT_ELAPSED_TIME);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix(); {
		camApply();

		int i;
		for (i=0; i<DOTS_CNT; i++) {
			glPushMatrix();
			move(dots[i], dotso[i]);
			glTranslatef(dots[i][0], dots[i][1], dots[i][2]);
			glutSolidSphere(0.1, 20, 20);
			glPopMatrix();
		}
		// plane
		glNormal3f(0, 1, 0);
		glBegin(GL_TRIANGLES);
		for (i=0; i<2; i++) {
			glVertex3fv(plane_vs[plane_fs[i][0]]);
			glVertex3fv(plane_vs[plane_fs[i][1]]);
			glVertex3fv(plane_vs[plane_fs[i][2]]);
		}
		glEnd();
	} glPopMatrix();

	// draw fps
	char buf[128];
	sprintf(buf, "spf: %.2f", spf);
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
	if (key['w']) camForward(+4 / fps);
	if (key['s']) camForward(-4 / fps);
	if (key['d']) camTurn(+90 / fps);
	if (key['a']) camTurn(-90 / fps);
	if (key['q']) camPan(+4 / fps);
	if (key['e']) camPan(-4 / fps);
	camEnd();

	spf = (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0f;
	fps = 1 / spf;
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Load Wavefront Obj");
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
	/* lighting */ {
		float light_pos[] = { 1, 1, 0, 0 };
		float white_light[] = { 0.8, 0.8, 0.8, 1 };

		glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
		glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

		glEnable(GL_LIGHT1);
	}

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	int i;
	for (i=0; i<DOTS_CNT; i++) {
		dotso[i][0] = dots[i][0] = (rand()%1000) / 400.0f + 1;
		dotso[i][1] = dots[i][1] = (rand()%1000) / 1000.0f;
		dotso[i][2] = dots[i][2] = (rand()%1000) / 400.0f;
	}

	camInit();

	glutMainLoop();

	return 0;
}

