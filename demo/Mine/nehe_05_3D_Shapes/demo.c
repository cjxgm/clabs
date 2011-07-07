#define DEMO_TITLE			"NeHe - 05 3D Shapes"
#define DEMO_WIN_SIZE		480, 480
#define DEMO_VIEW_RANGE		1, 1, 1
#define DEMO_FPS			30
#include "demolino.h"
#include <math.h>
#include <stdlib.h>
#define PI 3.1415927

float angle = 0;

// prototype
void draw(void);

int main(void)
{
	demo_main(DEMO_TITLE, DEMO_WIN_SIZE, DEMO_VIEW_RANGE, DEMO_FPS);
	return 0;
}

void init(void)
{
	demo_perspective(true);
}

void timer(int value)
{
	angle += 5;
}

void render(void)
{
	CLEAR;
	PUSH;
	glTranslatef(0, 0, 170);
	PUSH;
	SET_COLOR_RGB(0, 128, 255);
	glTranslatef(-90, 85, 0);
	glScalef(6, 6, 1);
	draw_logo();
	POP;
	draw();
	POP;    
	SWAP;
}

void draw(void)
{
	glRotatef(angle, 0, 1, 0);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f,0.0f,0.0f);			// Red
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top Of Triangle (Front)
	glColor3f(0.0f,1.0f,0.0f);			// Green
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Left Of Triangle (Front)
	glColor3f(0.0f,0.0f,1.0f);			// Blue
	glVertex3f( 1.0f,-1.0f,	1.0f);		// Right Of Triangle (Front)

	glColor3f(1.0f,0.0f,0.0f);			// Red
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top Of Triangle (Right)
	glColor3f(0.0f,0.0f,1.0f);			// Blue
	glVertex3f( 1.0f,-1.0f, 1.0f);		// Left Of Triangle (Right)
	glColor3f(0.0f,1.0f,0.0f);			// Green
	glVertex3f( 1.0f,-1.0f, -1.0f);		// Right Of Triangle (Right)

	glColor3f(1.0f,0.0f,0.0f);			// Red
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top Of Triangle (Back)
	glColor3f(0.0f,1.0f,0.0f);			// Green
	glVertex3f( 1.0f,-1.0f, -1.0f);		// Left Of Triangle (Back)
	glColor3f(0.0f,0.0f,1.0f);			// Blue
	glVertex3f(-1.0f,-1.0f, -1.0f);		// Right Of Triangle (Back)

	glColor3f(1.0f,0.0f,0.0f);			// Red
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top Of Triangle (Left)
	glColor3f(0.0f,0.0f,1.0f);			// Blue
	glVertex3f(-1.0f,-1.0f,-1.0f);		// Left Of Triangle (Left)
	glColor3f(0.0f,1.0f,0.0f);			// Green
	glVertex3f(-1.0f,-1.0f, 1.0f);		// Right Of Triangle (Left)
	glEnd();							// Done Drawing The Pyramid
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
	}
}

