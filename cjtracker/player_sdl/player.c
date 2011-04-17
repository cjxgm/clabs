/** player.c
 * CJTM Player: Player
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include "player.h"
#include "instrument.h"

#ifndef PI
#define PI 3.1415927
#endif

		byte	 snd_data = 0;
static	byte	 snds[2048] = {0};
static	byte	 playable = 0;

void mixaudio(void *unused, Uint8 *stream, int len);

int player_init(void)
{
	atexit(player_free);

	int tries = 10;

	while (tries--) {
		if (SDL_Init(SDL_INIT_AUDIO) < 0) {
			sleep(1);
			continue;
		}

		SDL_AudioSpec fmt;
		fmt.freq = 8000; //22050;
		fmt.format = AUDIO_U8;
		fmt.channels = 1;
		fmt.samples = 2048; //512;        /* A good value for games */
		fmt.callback = mixaudio;
		fmt.userdata = NULL;

		if (SDL_OpenAudio(&fmt, NULL) < 0) {
			sleep(1);
			continue;
		}

		return 0;
	}

	return -1;
}

void play(mod_t *mod)
{
	int p, c;
	int time = 0, tm = 0;
	note_t *note_playing[4] = {NULL};

	SDL_PauseAudio(0);

	for (p=0; p<64;) {
		for (c=0; c<4; c++) {
			note_t *note = &(mod->pats[0].chns[c].notes[p]);

			if (note->freq == 0 && note->note == 90)
				note_playing[c] = NULL;
			else if (note->freq != 0 && note_playing[c] != note) {
				note_playing[c] = note;
				note_playing[c]->frame = 0;
			}

			if (note_playing[c] != NULL) {
				mix_instrument(note_playing[c]->ins_id,
						note_playing[c]->freq * note_playing[c]->frame,
						note->vol);
				note_playing[c]->frame += 2.0 * PI / 2048.0;
				if (note_playing[c]->frame > 2.0 * PI * note_playing[c]->freq)
					note_playing[c]->frame -= 2.0 * PI * note_playing[c]->freq;
			}
		}

		snds[tm++] = snd_data;
		if (tm == 2048) {
			//sleep(1);
			playable = 1;
			while (playable);
			tm = 0;
		}

		time++;
		if (time >= 2048 * 60 / mod->bpm) {
			time = 0;
			p++;
		}
	}
}

void mixaudio(void *unused, Uint8 *stream, int len)
{
	if (!playable) return;
	SDL_MixAudio(stream, snds, 2048, SDL_MIX_MAXVOLUME);
	playable = 0;
}

void player_free(void)
{
	SDL_CloseAudio();
	SDL_Quit();
}

