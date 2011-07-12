/** demoutil.h
 * Demolino: Utilities
 **/
#ifndef __DEMOUTIL_H__
#define __DEMOUTIL_H__

#include <GL/glut.h> 

// TYpe
#ifndef bool
#define bool	char
#define true	1
#define false	0
#endif

#ifndef byte
#define byte	unsigned char
#endif

// Constant
#ifndef PI
#define PI 3.1415927
#endif

// Uilities
#define CLEAR glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
#define RGB(R, G, B) (float)(R)/255.0f, (float)(G)/255.0f, (float)(B)/255.0f
#define RGBA(R, G, B, A) RGB(R, G, B), (float)(A)/255.0f
#define SET_COLOR_RGB(R, G, B) glColor3f(RGB(R, G, B))
#define SET_COLOR_RGBA(R, G, B, A) glColor4f(RGBA(R, G, B, A))
#define PUSH glPushMatrix()
#define POP glPopMatrix()
#define SWAP glutSwapBuffers()

void draw_circle(float x, float y, float rx, float ry, bool fill);
void draw_square(float x1, float y1, float x2, float y2, bool fill);
void draw_logo(void);

#endif

