
#include "camera.h"
#include <GL/gl.h>

float camera[16];

void camInit(void)
{
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, camera);
	glPopMatrix();
}

void camApply(void)
{
	glLoadMatrixf(camera);
}

void camBegin(void)
{
	glPushMatrix();
}

void camEnd(void)
{
	glPopMatrix();
}

void camForward(float howmuch)
{
	glLoadIdentity();
	glTranslatef(0, 0, howmuch);
	glMultMatrixf(camera);
	glGetFloatv(GL_MODELVIEW_MATRIX, camera);
}

void camPan(float howmuch)
{
	glLoadIdentity();
	glTranslatef(howmuch, 0, 0);
	glMultMatrixf(camera);
	glGetFloatv(GL_MODELVIEW_MATRIX, camera);
}

void camTurn(float howmuch)
{
	glLoadIdentity();
	glRotatef(howmuch, 0, 1, 0);
	glMultMatrixf(camera);
	glGetFloatv(GL_MODELVIEW_MATRIX, camera);
}

void camGetPosition(float pos[])
{
	pos[0] = camera[0]*camera[12]
			+ camera[1]*camera[13]
			+ camera[2]*camera[14];
	pos[1] = camera[4]*camera[12]
			+ camera[5]*camera[13]
			+ camera[6]*camera[14];
	pos[2] = camera[8]*camera[12]
			+ camera[9]*camera[13]
			+ camera[10]*camera[14];
}

