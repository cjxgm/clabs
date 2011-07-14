/** demosnd.h
 * Demolino: Sound
 **/
#include <SDL/SDL_audio.h>
#include <stdio.h>
#include "demosnd.h"

extern void mixaudio(void *unused, Uint8 *stream, int len);

int snd_init(void)
{
	atexit(snd_destroy);
	if (SDL_Init(SDL_INIT_AUDIO) < 0){
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return -1;
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
	if (SDL_OpenAudio(&fmt, NULL) < 0) {
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		return -1;
	}

	SDL_PauseAudio(0);
	return 0;
}

void snd_destroy(void)
{
	SDL_CloseAudio();
	SDL_Quit();
}

void mixaudio(void *unused, Uint8 *stream, int len)
{
	if (snd_len == 0) return;
	SDL_MixAudio(stream, snd_data, snd_len, SDL_MIX_MAXVOLUME);
}

