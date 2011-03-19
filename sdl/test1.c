#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h" 
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h" 

static SDL_Surface *screen;

int main(int argc, char *argv[])
{
	int quit = 0;
	SDL_Surface *text_sur;
	SDL_Surface *background; 
	SDL_Event event;
	SDL_Color color;
	SDL_Rect srect, drect;
	Mix_Music *bgsound;
	TTF_Font *font;

	atexit(SDL_Quit);
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		fprintf(stderr, "SDL init error:%s\n", SDL_GetError());
		exit(0);
	}

	screen = SDL_SetVideoMode(800, 600, 24, SDL_HWSURFACE);
	if (screen == NULL) 
	{
		fprintf(stderr, "Set video mode failure:%s\n", SDL_GetError());
		exit(0);
	}
	SDL_WM_SetCaption("test", NULL);

	if (TTF_Init() < 0) 
	{
		fprintf(stderr, "TTF init error:%s\n", SDL_GetError());
		return;
	}

	background = IMG_Load("background.jpg");
	printf("%d\n", (int)background);

	srect.x = 0;
	srect.y = 0;
	srect.w = background->w;
	srect.h = background->h;

	drect = srect;
	SDL_BlitSurface(background, &srect, screen, &drect);

	font = TTF_OpenFont("test.ttf", 40);

	color.r = 255;
	color.g = 0;
	color.b = 0;

	text_sur=TTF_RenderText_Solid(font, "Hello, Welcome to GAME!!", color);

	srect.x = 0;
	srect.y = 0;
	srect.w = text_sur->w;
	srect.h = text_sur->h;

	drect.x = (800 - text_sur->w) / 2;
	drect.y = (600 - text_sur->h) / 2;
	drect.w = text_sur->w;
	drect.h = text_sur->h;

	SDL_BlitSurface(text_sur, &srect, screen, &drect);
	SDL_UpdateRect(screen, 0, 0, 800, 600);

	Mix_OpenAudio(44100, AUDIO_S16, 2, 4096);
	bgsound = Mix_LoadMUS("bgsound.mp3");
	Mix_PlayMusic(bgsound, -1);

	while (quit == 0) 
	{
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
				case SDL_QUIT:
					quit = 1;
					break;
				default:
					break;
			}
		}
		SDL_Delay(100);
	}

	return 0;
}
