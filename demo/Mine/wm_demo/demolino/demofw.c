#include <GL/glut.h> 
#include "demofw.h"

unsigned int time = 0;
unsigned int time_base = 0;
static bool perspective = false;

static int win_w, win_h;
static int _view_x, _view_y, _view_z;

static void idle_func()
{
	time = glutGet(GLUT_ELAPSED_TIME) - time_base;
	glutPostRedisplay();
}

static void change_size(int w, int h)
{
	win_w = w;
	win_h = h;

	// Prevent a divide by zero
	if(h == 0) h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (perspective){
		gluPerspective(60.0f, (float)w / h, 1, 400);
		glTranslatef(0, 0, -174);
	}
	else {
		// Establish clipping volume (left, right, bottom, top, near, far)
		if (w <= h)
			glOrtho(-_view_x, _view_x, 
						-_view_y*h/w, _view_y*h/w,
						-_view_z, _view_z);
		else
			glOrtho(-_view_x*w/h, _view_x*w/h,
						-_view_y, _view_y,
						-_view_z, _view_z);
	}

	// Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void demo_main(const char *title, int width, int height,
				float view_x, float view_y, float view_z, float fps)
{
	_view_x = view_x;
	_view_y = view_y;
	_view_z = view_z;
	int argc = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow(title);
	glutReshapeFunc(change_size);
	glutDisplayFunc(render);
	glutSpecialFunc(special_press);
	glutKeyboardFunc(key_press);
	glutIdleFunc(idle_func);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0, 0, 0, 0);

	init();

	glutMainLoop();
}

void demo_perspective(bool persp)
{
	perspective = persp;
	change_size(win_w, win_h);
}

