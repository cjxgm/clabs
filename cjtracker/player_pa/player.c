/** player.c
 * CJTM Player: Player
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "driver_pa.h"
#include "instrument.h"

#ifndef PI
#define PI 3.1415927
#endif

byte	 snd_data = 0;

int player_init(void)
{
	return sound_init();
}

void play(mod_t *mod)
{
	int p, c;
	int time = 0;
	note_t *note_playing[4] = {NULL};

	for (p=0; p<64;) {
		snd_data = 0;
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

		sound_play(snd_data);

		time++;
		if (time >= 3000 * 60 / mod->bpm) {
			time = 0;
			p++;
		}
	}

	sound_wait();
}

void player_free(void)
{
	sound_free();
}

