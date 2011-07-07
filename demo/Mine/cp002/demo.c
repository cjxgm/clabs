
#define DEMO_TITLE "LanDeXithO 002 - Waves"
#define DEMO_WIN_SIZE 480, 480
#include "demolino.h"
#include <math.h>
#define PI 3.1415927

#define WAVE_SIZE 10
struct wave
{
	int type; // 0: Sine; 1: Cosine;
	float angle;
	float angle_inc;
	float x, y;
	float x_inc;
	float y_scale;
	int r, g, b;
} waves[WAVE_SIZE];

void circle(float x, float y, float r)
{
	glBegin(GL_POLYGON);
	float a;
	for (a=0; a<2*PI; a+=0.1)
		glVertex2f(x+cos(a)*r, y+sin(a)*r);
	glEnd();
}

void draw_wave_path(int i)
{
	glBegin(GL_LINE_STRIP);
	float a, x;
	for (a=0, x=-100; x<=100; 
			a+=waves[i].angle_inc, x+=waves[i].x_inc){
		float y_offset = 0;
		switch (waves[i].type){
			case 0: // Sine
				y_offset = sin(a);
				break;
			case 1: // Cosine
				y_offset = cos(a);
				break;
		}
		glVertex2f(x, waves[i].y + y_offset*waves[i].y_scale);
	}
	glEnd();
}

void draw(void)
{
	int i;
	for (i=0; i<WAVE_SIZE; i++){
		float y_offset = 0;
		switch (waves[i].type){
			case 0: // Sine
				y_offset = sin(waves[i].angle);
				break;
			case 1: // Cosine
				y_offset = cos(waves[i].angle);
				break;
		}
		SET_COLOR_RGBA(waves[i].r, waves[i].g, waves[i].b, 128);
		draw_wave_path(i);
		circle(waves[i].x, waves[i].y + y_offset*waves[i].y_scale, 2);
	}
}

void render(void)
{
	CLEAR;
	PUSH;
	draw();
	POP;    
	SWAP;
}

void init_wave(int i)
{
	waves[i].type = rand()%2;
	waves[i].angle = 0;
	waves[i].angle_inc = (float)(rand()%10)*PI/180;
	waves[i].x = -100;
	waves[i].y = (rand()%200)-100;
	waves[i].x_inc = (rand()%6) + 1;
	waves[i].y_scale = (rand()%100)/10.0 + 1;
	waves[i].r = (rand()%192) + 64;
	waves[i].g = (rand()%192) + 64;
	waves[i].b = (rand()%192) + 64;
}

void update_waves(void)
{
	int i;
	for (i=0; i<WAVE_SIZE; i++){
		waves[i].angle += waves[i].angle_inc;
		waves[i].x+=waves[i].x_inc;
		if (waves[i].x>100) init_wave(i);
	}
}

void timer(int value)
{
	update_waves();
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
	int i;
	for (i=0; i<WAVE_SIZE; i++){
		init_wave(i);
	}
}

int main(void)
{
	demo_main(DEMO_TITLE, DEMO_WIN_SIZE, 100, 100, 100, 30);
	return 0;
}

void keypress(byte key, int x, int y)
{
	switch (key){
		case 27: // Esc
			exit(0);
	}
}

