#define DEMO_TITLE			"Clanjor Production"
#define DEMO_WIN_SIZE		480, 480
#define DEMO_VIEW_RANGE		100, 100, 100
#define DEMO_FPS			30
#include "demolino.h"
#include <math.h>
#include <stdlib.h>
#define PI 3.1415927

// prototype
void draw(void);

int main(void)
{
	demo_main(DEMO_TITLE, DEMO_WIN_SIZE, DEMO_VIEW_RANGE, DEMO_FPS);
	return 0;
}

void init(void)
{
}

void timer(int value)
{
}

void render(void)
{
	CLEAR;
	PUSH;
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
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
	}
}

