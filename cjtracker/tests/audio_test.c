#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <math.h>
#include <stdio.h>
#ifndef PI
#define PI 3.1415927
#endif

float alpha = 0;
static Uint8 data[2048];
float note[27] = {8, 8, 8, 8, 8, 6, 4, 4, 4, 4, 5, 6, 7, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
int np=0;

extern void mixaudio(void *unused, Uint8 *stream, int len);

int main(void)
{
	atexit(SDL_Quit);
	if (SDL_Init(SDL_INIT_AUDIO) < 0){
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_AudioSpec fmt;
	/* Set 16-bit stereo audio at 22Khz */
	fmt.freq = 22050;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 512;        /* A good value for games */
	fmt.callback = mixaudio;
	fmt.userdata = NULL;

	/* Open the audio device and start playing sound! */
	if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_PauseAudio(0);
	for(;;);
	SDL_CloseAudio();

	SDL_Quit();
	return 0;
}

void mixaudio(void *unused, Uint8 *stream, int len)
{
	int i;
	for (i=0; i<2048; i++, alpha+=0.1){
		data[i] = sin(alpha/note[np])*32;
	}
	SDL_MixAudio(stream, data, 2048, SDL_MIX_MAXVOLUME);
	np++;
	if (np==27) np = 0;
}

