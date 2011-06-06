#include "demolino.h"
#include <math.h>

#define DEMO_TITLE		"OpenGL Learning"
#define DEMO_WIN_SIZE	640, 480

int n = 1;				// day
float rotate_x = 20.0;
float rotate_y = 10.0;
float rotate_z = 22.0;
float forward = 0.0;
bool t = true;			// show text

// prototype
void draw(void);
void draw_circle_z(float size);

int main(void)
{
	printf("OpenGL Learning\n");
	printf("    Coded by eXerigumo Clanjor\n");
	printf("    Under GPLv3, ABSOLUTELY NO WARRANTY!!!\n");
	printf("Keys:\n");
	printf("    t       Enable/Disable text\n");
	printf("    w       Rotate along x-axis\n");
	printf("    s       Rotate along x-axis\n");
	printf("    a       Rotate along y-axis\n");
	printf("    d       Rotate along y-axis\n");
	printf("    q       Rotate along z-axis\n");
	printf("    e       Rotate along z-axis\n");
	printf("    r       Go forward\n");
	printf("    f       Go backward\n");
	printf("    ESC     Exit\n");
	fflush(stdout);

	demo_main(DEMO_TITLE,		// title
				DEMO_WIN_SIZE,	// w, h
				100,			// view_w
				100,			// view_h
				100,			// view_z
				30);			// fps

	return 0;
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
	demo_perspective(true);

	GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_light_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat sun_light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void timer(int value)
{
	n++;
	if (n > 360) n = 1;
}

void render(void)
{
	CLEAR;

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
	draw();
	POP;

	SWAP;
}

char buf[512] = "";
void draw(void)
{

	// Draw <n>
	sprintf(buf, "%d\n%d %d %d",
				n, (int)rotate_x,
				(int)rotate_y, (int)rotate_z);
	glColor3f(1.0, 0.8, 0.4);
	PUSH;
	glTranslatef(-20, 90, 0);
	glScalef(5, 4, 1);
	draw_string(buf);
	POP;

	glTranslatef(0, 0, forward);
	glRotatef(rotate_x, 1, 0, 0);
	glRotatef(rotate_y, 0, 1, 0);
	glRotatef(rotate_z, 0, 0, 1);

	// Sun
	glRotatef(n, 0, 1, 0);
	//glColor3f(1.0, 0.8, 0.4);
	{
		GLfloat sun_mat_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_mat_diffuse[]  = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_mat_emission[] = {0.5f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_mat_shininess  = 0.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT,   sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION,  sun_mat_emission);
		glMaterialf (GL_FRONT, GL_SHININESS, sun_mat_shininess);

		glutSolidSphere(10, 20, 20);
	}
	//glColor3f(1.0, 1.0, 1.0);
	draw_circle_z(90);
	if (t) {
		PUSH;
		glTranslatef(16, 0, 0);
		glScalef(6, 4, 1);
		draw_string("sun");
		POP;
	}

	// Earth
	glTranslatef(90, 0, 0);
	//glColor3f(0.2, 0.4, 0.8);
	{
		GLfloat earth_mat_ambient[]  = {0.0f, 0.0f, 0.5f, 1.0f};
		GLfloat earth_mat_diffuse[]  = {0.0f, 0.0f, 0.5f, 1.0f};
		GLfloat earth_mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
		GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat earth_mat_shininess  = 30.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT,   earth_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   earth_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  earth_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION,  earth_mat_emission);
		glMaterialf (GL_FRONT, GL_SHININESS, earth_mat_shininess);

		glutSolidSphere(6, 20, 20);
	}
	//glColor3f(1.0, 1.0, 1.0);
	draw_circle_z(10);
	if (t) {
		PUSH;
		glTranslatef(16, 0, 0);
		glScalef(4, 2, 1);
		draw_string("earth");
		POP;
	}

	// Moon
	glRotatef(11 * n, 0, 1, 0);
	glTranslatef(10, 0, 0);
	glColor3f(0.4, 0.4, 0.4);
	glutSolidSphere(2, 20, 20);
	if (t) {
		glColor3f(1.0, 1.0, 1.0);
		PUSH;
		glTranslatef(16, 0, 0);
		glScalef(2, 1, 1);
		draw_string("moon");
		POP;
	}
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
		case 't': t = !t; break;
		case 'w': rotate_x += 1; break;
		case 's': rotate_x -= 1; break;
		case 'a': rotate_y += 1; break;
		case 'd': rotate_y -= 2; break;
		case 'q': rotate_z += 1; break;
		case 'e': rotate_z -= 1; break;
		case 'r': forward += 1; break;
		case 'f': forward -= 1; break;
		default:
			fprintf(stderr, "key: %d\n", key);
	}
}

// Draw a circle (line only) laid on the z-plane.
void draw_circle_z(float size)
{
	float a;

	glBegin(GL_LINE_LOOP);
	for (a=0; a<2*PI; a+=0.05)
		glVertex3f(size * cos(a), 0, size * sin(a));
	glEnd();
}

