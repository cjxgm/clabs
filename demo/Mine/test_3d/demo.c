#define DEMO_TITLE "Test - 3D"
#define DEMO_WIN_SIZE 480, 480
#include "demolino.h"
#include <math.h>
#include <stdlib.h>
#define PI 3.1415927

static float alpha = 0;
typedef struct vector3d
{
	float x, y, z;
} vector3d_t;
static vector3d_t square[] = {
	// Left
	{ -20, 20, 20 },
	{ -20, 20, -20 },
	{ -20, -20, -20 },
	{ -20, -20, 20 },
	// Right
	{ 20, 20, 20 },
	{ 20, 20, -20 },
	{ 20, -20, -20 },
	{ 20, -20, 20 },
	// Top
	{ -20, 20, 20 },
	{ 20, 20, 20 },
	{ 20, 20, -20 },
	{ -20, 20, -20 },
	// Bottom
	{ -20, -20, 20 },
	{ 20, -20, 20 },
	{ 20, -20, -20 },
	{ -20, -20, -20 },
};

// prototype
void draw(void);
void draw_face(vector3d_t face[], int len);
float tr_x(float x, float z);
float tr_y(float y, float z);
void translate(vector3d_t points[], int len, 
				float x, float y, float z);
vector3d_t *duplicate(vector3d_t src[], int len);
void rotate_x(vector3d_t points[], int len, float a);
void rotate_y(vector3d_t points[], int len, float a);
void rotate_z(vector3d_t points[], int len, float a);

int main(void)
{
	demo_main(DEMO_TITLE, DEMO_WIN_SIZE, 100, 100, 100, 30);
	return 0;
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
}

void timer(int value)
{
	alpha += 0.1;
	if (alpha > 2*PI) alpha -= 2*PI;
}

void render(void)
{
	CLEAR;
	PUSH;
	PUSH;
	SET_COLOR_RGB(0, 255, 128);
	glTranslatef(-30, 70, 0);
	glScalef(6, 6, 1);
	draw_logo();
	POP;
	draw();
	POP;    
	SWAP;
}

void draw(void)
{
	SET_COLOR_RGB(255, 128, 0);
	int i;
	for (i=0; i<4*4; i+=4){
	vector3d_t *face = duplicate(&square[i], 4);
	rotate_y(face, 4, alpha);
	//rotate_x(face, 4, alpha);
	//translate(face, 4, sin(alpha)*40, cos(alpha)*40, -40);
	translate(face, 4, 0, -30, -20);
	draw_face(face, 4);
	free(face);
	}
}

void draw_face(vector3d_t face[], int len)
{
	glBegin(GL_LINE_LOOP);
	int i;
	for (i=0; i<len; i++)
		glVertex2f(tr_x(face[i].x, face[i].z),
					tr_y(face[i].y, face[i].z));
	glEnd();
}

float tr_x(float x, float z)
{
	z = -z+200;
	if (z<1) z = 1;
	return x / z * 200;
}

float tr_y(float y, float z)
{
	z = -z+200;
	if (z<1) z = 1;
	return y / z * 200;
}

void translate(vector3d_t points[], int len, 
				float x, float y, float z)
{
	int i;
	for (i=0; i<len; i++){
		points[i].x += x;
		points[i].y += y;
		points[i].z += z;
	}
}

vector3d_t *duplicate(vector3d_t src[], int len)
{
	vector3d_t *dest = malloc(sizeof(vector3d_t)*len);
	int i;
	for (i=0; i<len; i++)
		dest[i] = src[i];
	return dest;
}

void rotate_x(vector3d_t points[], int len, float a)
{
	int i;
	for (i=0; i<len; i++){
		float nz = points[i].z*cos(a) - points[i].y*sin(a);
		float ny = points[i].y*cos(a) + points[i].z*sin(a);
		points[i].z = nz;
		points[i].y = ny;
	}
}

void rotate_y(vector3d_t points[], int len, float a)
{
	int i;
	for (i=0; i<len; i++){
		float nx = points[i].x*cos(a) - points[i].z*sin(a);
		float nz = points[i].z*cos(a) + points[i].x*sin(a);
		points[i].x = nx;
		points[i].z = nz;
	}
}

void rotate_z(vector3d_t points[], int len, float a)
{
	int i;
	for (i=0; i<len; i++){
		float nx = points[i].x*cos(a) - points[i].y*sin(a);
		float ny = points[i].y*cos(a) + points[i].x*sin(a);
		points[i].x = nx;
		points[i].y = ny;
	}
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
	}
}

