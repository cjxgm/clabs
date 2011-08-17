
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

float spin = 0;
static const float no[] = { 0, 0, 0, 1 };
float pos_x = 0, pos_y = 0, pos_z = 0;
static char key[256] = {0};

void draw(void)
{
	float light_pos0[] = { 0, 0, 0, 1 };
	float light_pos1[] = { 6, 0, 0, 0 };

	glShadeModel(GL_SMOOTH);

	glPushMatrix(); {
		glTranslatef(pos_x, pos_y, pos_z);
		glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

		glFrontFace(GL_CCW);
		// right-side light
		{
			float emission[] = { 0.75, 0.5, 0, 1 };
			glMaterialfv(GL_FRONT, GL_SPECULAR, no);
			glMaterialfv(GL_FRONT, GL_SHININESS, no);
			glMaterialfv(GL_FRONT, GL_EMISSION, emission);
		}
		glPushMatrix(); {
			glRotatef(-spin, 0, 1, 0);
			glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
			glTranslatef(6, 0, 0);
			glutSolidSphere(0.3, 10, 10);
		} glPopMatrix();

		glPushMatrix(); {
			glRotatef(spin, 0, 1, 0);

			// Center light
			{
				float emission[] = { 1, 1, 1, 1 };
				glMaterialfv(GL_FRONT, GL_SPECULAR, no);
				glMaterialfv(GL_FRONT, GL_SHININESS, no);
				glMaterialfv(GL_FRONT, GL_EMISSION, emission);
			}
			glutSolidSphere(0.3, 10, 10);

			{
				float mat_specular[] = { 1, 1, 1, 1 };
				float mat_shininess[] = { 50 };
				glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
				glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
				glMaterialfv(GL_FRONT, GL_EMISSION, no);
			}

			glFrontFace(GL_CW);
			glShadeModel(GL_FLAT);
			glPushMatrix(); {
				glTranslatef(-2, 2, 0);
				glRotatef(spin, 0.1, 1, 0);
				glutSolidTeapot(2.0);
			} glPopMatrix();

			glFrontFace(GL_CCW);
			glShadeModel(GL_SMOOTH);
			glPushMatrix(); {
				glTranslatef(-2, -2, 0);
				glRotatef(spin, 0.1, 1, 0);
				glRotatef(30, 1, 0, 0);
				glutSolidCube(1.0);
			} glPopMatrix();

			glPushMatrix(); {
				glTranslatef(2, 0, 0);
				glRotatef(spin, 0.1, 1, 0);
				glRotatef(30, 1, 0, 0);
				glutSolidSphere(1.0, 80, 80);
			} glPopMatrix();
		} glPopMatrix();
	} glPopMatrix();

	spin += 0.5;

	if (key['q']) pos_z += 0.1;
	if (key['e']) pos_z -= 0.1;
	if (key['w']) pos_y -= 0.1;
	if (key['s']) pos_y += 0.1;
	if (key['a']) pos_x += 0.1;
	if (key['d']) pos_x -= 0.1;
	if (key[27]) exit(0);
}

void key_down(unsigned char k, int x, int y)
{
	key[k] = 1;
}

void key_up(unsigned char k, int x, int y)
{
	key[k] = 0;
}

void render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw();

	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	draw();

	glutSwapBuffers();
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Lighting");
	glutIdleFunc(&render);
	glutKeyboardFunc(&key_down);
	glutKeyboardUpFunc(&key_up);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 640.0 / 480.0, 1, 100);
	glTranslatef(0, 0, -10);
	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	/* lighting & material */ {
		float light_pos[] = { 0, 0, 0, 1 };
		float white_light[] = { 1, 1, 1, 1 };
		float ambient[] = { 0.1, 0.1, 0.1, 1 };

		glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
		glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	/* lighting */ {
		float light_pos[] = { 2, 0, 1, 0 };
		float white_light[] = { 0.75, 0.5, 0, 1 };

		glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);

		glEnable(GL_LIGHT1);
	}

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glutMainLoop();

	return 0;
}

