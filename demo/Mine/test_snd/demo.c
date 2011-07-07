#define DEMO_TITLE "Test - Sound"
#define DEMO_WIN_SIZE 480, 480
#include "demolino.h"
#include <math.h>
#include <stdlib.h>
#define PI 3.1415927

static float alpha = 0;
static int loop = 0;
static int tick = 0;
static float k = 1;
static int p = 0;
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
	/*                  12 ___
		1.059463 ~= 1 +  ./ 2
	 */
	k = 1;
	int data[] = { 40, 42, 44, 45, 47, 49, 51, 52};
	loop = data[p];
	while(loop--) k *= 1.059463;
	for (loop = 0; loop<2048; loop++){
		snd_data[loop] = (sin(k*alpha)+1)*10;
		if (snd_len != 2048) snd_len++;
		alpha += 2.0 * PI / 2048.0;
		if (alpha > 2.0 * PI / k) alpha -= 2.0 * PI / k;
	}
	if (tick == 10){
		tick = 0;
		p++;
		p %= 8;
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
	glScalef(4, 4, 1);
	char text[80] = "";
	sprintf(text, "Sine Generator\n%f %d\t%f", k, p, alpha);
	draw_string(text);
	POP;
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
	}
}

