/** player.c
 * CJTM Player: Player
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pulse/simple.h>
#include "player.h"
#include "instrument.h"

#ifndef PI
#define PI 3.1415927
#endif

		byte	 snd_data = 0;
static	byte	 snds[2048] = {0};
static	int		 tm = 0;

static	pa_simple		*s	= NULL;
static	pa_sample_spec	 ss	= {
	.format		= PA_SAMPLE_U8,
	.rate		= 8000,
	.channels	= 1
};

int player_init(void)
{
	int tries = 3;

	while (tries--) {
		s = pa_simple_new(NULL,
						"CJTM Player",
						PA_STREAM_PLAYBACK,
						NULL,
						"Clanjor Tracker Module",
						&ss,
						NULL,
						NULL,
						NULL);
		if (s == NULL) {
			sleep(1);
			continue;
		}

		return 0;
	}

	return -1;
}

void play(mod_t *mod)
{
	int err = 0;
	int p, c;
	int time = 0;
	note_t *note_playing[4] = {NULL};
	tm = 0;

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
			pa_simple_write(s,
							snds,
							tm,
							&err);
			tm = 0;
		}

		time++;
		if (time >= 2048 * 60 / mod->bpm) {
			time = 0;
			p++;
		}
	}

	if (tm != 0) {
		pa_simple_write(s,
						snds,
						tm,
						&err);
	}

	pa_simple_drain(s, &err); // Wait for all datas to be played.
}

void player_free(void)
{
	if (s) pa_simple_free(s);
}

