#include <GL/glut.h> 
#include "demofw.h"

static float spf = 1000.0 / 30.0; // Seconds per Frame (default 30 fps)
static bool perspective = false;

static int win_w, win_h;
static int _view_x, _view_y, _view_z;

void TimerFunction(int value)
{
	timer(value);

		glutPostRedisplay();
		glutTimerFunc(spf, TimerFunction, 0);
}

void SpecialKeys(int key, int x, int y)
{
	/*
	if(key == GLUT_KEY_UP) xRot-= 5.0f;
	if(key == GLUT_KEY_DOWN) xRot += 5.0f;
	if(key == GLUT_KEY_LEFT) yRot -= 5.0f;
	if(key == GLUT_KEY_RIGHT) yRot += 5.0f;
	if(key > 356.0f) xRot = 0.0f;
	if(key < -1.0f) xRot = 355.0f;
	if(key > 356.0f) yRot = 0.0f;
	if(key < -1.0f) yRot = 355.0f;

	// Refresh the Window
	glutPostRedisplay();
	*/
}

void ChangeSize(int w, int h)
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

extern void demo_main(const char *title, int width, int height,
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
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(render);
	// glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(keypress);
	spf = 1000.0 / fps;
	glutTimerFunc(spf, TimerFunction, 0);

	atexit(demo_destroy);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(0, 0, 0, 0);

	init();

	glutMainLoop();
}

extern void demo_destroy(void)
{
}

extern void demo_perspective(bool persp)
{
	perspective = persp;
	ChangeSize(win_w, win_h);
}

