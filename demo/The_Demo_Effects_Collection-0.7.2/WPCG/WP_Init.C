#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "WP_Init.h"

namespace WPCG
{
WP_Init::WP_Init(int argc, char* argv[]){
	glutInit(&argc, argv);
}	

WP_Init::~WP_Init(){}

void WP_Init::vSetWindow(scalar left, scalar right, scalar bottom, scalar top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, top, bottom); 
}

void WP_Init::vSetViewPort(GLint left, GLint right, GLint bottom, GLint top)
{
	glViewport(left, top, right - left, bottom);
}
}
