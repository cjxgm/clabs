/** player.c
 * CJTM Player: Player
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "player.h"
#include "instrument.h"

#ifndef PI
#define PI 3.1415927
#endif

static FILE			*dspout;

byte	snd_data = 0;

int player_init(void)
{
	int tries = 10;

	while (tries--) {
		if ((dspout = fopen("/dev/dsp", "wb")) != NULL) {
			return 0;
		}
		sleep(1);
	}

	return -1;
}

void play(mod_t *mod)
{
	int p, c;
	int time = 0;
	note_t *note_playing[4] = {NULL};

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

		time++;
		fwrite(&snd_data, 1, 1, dspout);

		if (time >= 2048 * 60 / mod->bpm) {
			time = 0;
			p++;
		}
	}
}

void player_free(void)
{
	fclose(dspout);
}

