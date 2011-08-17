
#include "camera.h"
#include <GL/gl.h>

static float camera[16];

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

