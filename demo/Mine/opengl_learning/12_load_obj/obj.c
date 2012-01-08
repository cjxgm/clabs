
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "obj_loader.h"

static const float no[] = { 0, 0, 0, 1 };
static char key[256] = {0};
static float fps = 1;
static Obj * go = NULL;

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

void render(void)
{
	unsigned int time = glutGet(GLUT_ELAPSED_TIME);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix(); {
		camApply();
		glTranslatef(0, 0, -5);
		glRotatef(90, 1, 0, 0);
		draw_obj(go);
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
	if (key['w']) camForward(+4 / fps);
	if (key['s']) camForward(-4 / fps);
	if (key['d']) camTurn(+90 / fps);
	if (key['a']) camTurn(-90 / fps);
	if (key['q']) camPan(+4 / fps);
	if (key['e']) camPan(-4 / fps);
	camEnd();

	fps = 1000.0 / (glutGet(GLUT_ELAPSED_TIME) - time);
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

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	camInit();

	FILE * fp = fopen("xxx.obj", "r");
	go = load_obj(fp);
	fclose(fp);

	glutMainLoop();

	return 0;
}

