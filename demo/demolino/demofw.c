#include <GL/glut.h> 
#include "demofw.h"

static float spf = 33.3333; // Seconds per Frame (default 30 fps)

void TimerFunction(int value)
{
	timer(value);

	glutPostRedisplay();
	glutTimerFunc(spf, TimerFunction, 1);
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
	float nRange = 100.0f;

	// Prevent a divide by zero
	if(h == 0) h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)  glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
	else  glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

	// Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

extern void demo_main(const char *title,
				int width, int height, float fps)
{
	int argc = 0;
	glutInit(&argc, NULL);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow(title);
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(render);
	glutSpecialFunc(SpecialKeys);
	spf = 1000.0 / fps;
	glutTimerFunc(spf, TimerFunction, 1);

	init();

	glutMainLoop();
}

