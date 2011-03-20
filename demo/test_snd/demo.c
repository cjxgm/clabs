#define DEMO_TITLE "Test - Sound"
#define DEMO_WIN_SIZE 480, 480
#include "demolino.h"
#include <math.h>
#include <stdlib.h>
#define PI 3.1415927

static float alpha = 0;
static float k = 0.1;
static int loop = 0;
static int tick = 0;
int snd_len = 0;
Uint8 snd_data[2048] = {0};

// prototype
void draw(void);

int main(void)
{
	demo_main(DEMO_TITLE, DEMO_WIN_SIZE, 100, 100, 100, 30);
	snd_destroy();
	return 0;
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
	snd_init();
}

void timer(int value)
{
	for (loop = 0; loop<2048; loop++){
		snd_data[loop] = (sin(alpha)+1)*10;
		if (snd_len != 2048) snd_len++;
		alpha += k;
		if (alpha > 2*PI) alpha -= 2*PI;
	}
	if (tick == 30){
		k = (sin(alpha)+1)/2*1.8/10+0.02;
		tick = 0;
	}
	tick++;
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
	glBegin(GL_LINE_STRIP);
	for (i=0; i<2048; i++)
		glVertex2f((i/1024.0-1)*100, snd_data[i]-5);
	glEnd();

	SET_COLOR_RGB(128, 64, 255);
	PUSH;
	glTranslatef(-80, -80, 0);
	glScalef(8, 8, 1);
	draw_string("Sine Generator");
	POP;
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
	}
}

