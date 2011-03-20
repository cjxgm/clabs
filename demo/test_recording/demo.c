#define DEMO_TITLE "Test - Recording"
#define DEMO_WIN_SIZE 480, 480
#include "demolino.h"
#include <stdio.h>
#include <stdlib.h>

// prototype
void draw(void);

unsigned char data[200] = {0};
int data_p = 0;
FILE *fpin = NULL;

int main(void)
{
	demo_main(DEMO_TITLE, DEMO_WIN_SIZE, 100, 100, 100, 30);
	fclose(fpin);
	return 0;
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
	fpin = fopen("/dev/dsp", "rb");
	if (!fpin) exit(0);
}

void timer(int value)
{
	fread(&data[data_p], 1, 50, fpin);
	data_p += 50;
	if (data_p >= 200) data_p = 0;
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
	for (i=0; i<200; i++)
		glVertex2f(i-100, (data[i]-128)/128.0 * 20);
	glEnd();

	SET_COLOR_RGB(128, 64, 255);
	PUSH;
	glTranslatef(-80, -80, 0);
	glScalef(8, 8, 1);
	draw_string("Recorder");
	POP;
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
	}
}

