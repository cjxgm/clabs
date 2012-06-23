
/************************************************************
 * cp: video.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include "video.h"
#include "../demo/demo.h"
#include <GL/glut.h>

int video_w = 640;
int video_h = 480;

static int win;

static void init();
static void idle();
static void display();
static void resize(int w, int h);
static void keypress(unsigned char key, int x, int y);

void video_init()
{
	// init glut
	int argc = 1;
	char * argv[] = {""};	// this sucks!!!
	glutInit(&argc, argv);

	// create render window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(video_w, video_h);
	win = glutCreateWindow("cp");

	// bind events
	glutDisplayFunc(&display);
	glutReshapeFunc(&resize);
	glutKeyboardFunc(&keypress);
	glutIdleFunc(&init);

	// setup blank color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// setup anti-aliasing for points and lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glutMainLoop();
}

void video_close()
{
	demo_close();
	glutDestroyWindow(win);
}

static void init()
{
	demo_init();
	glutIdleFunc(&idle);
}

static void idle()
{
	glutPostRedisplay();
}

static void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

static void resize(int w, int h)
{
	video_w = w;
	video_h = h;
}

static void keypress(unsigned char key, int x, int y)
{
	if (key == '\e') video_close();
}

