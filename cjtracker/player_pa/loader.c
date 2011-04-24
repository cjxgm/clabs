/** loader.c
 * CJTM Player: Loader
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "loader.h"
#include "player.h"

static char *note_table[12] = {
	"C-", "C#",
	"D-", "D#",
	"E-",
	"F-", "F#",
	"G-", "G#",
	"A-", "A#",
	"B-"
};

static int note_str_to_ub(const char *note_str);

static int load_header(FILE *fp, mod_t *mod);
static int load_pattern(FILE *fp, mod_t *mod);

void play_mod_file(FILE *fp)
{
	mod_t	*mod;
	
	if (fp == NULL)
		return;
	
	if ((mod = calloc(sizeof(mod_t), 1)) == NULL)
		return;

	if (load_header(fp, mod) == -1
		|| player_init() == -1) {
		free(mod);
		return;
	}

	while (load_pattern(fp, mod) == 0)
		play(mod);

	player_free();
	free(mod);
}

mod_t *load(FILE *fp)
{
	mod_t	*mod;
	
	if (fp == NULL)
		return NULL;
	
	if ((mod = calloc(sizeof(mod_t), 1)) == NULL)
		return NULL;

	if (load_header(fp, mod) == -1) {
		free(mod);
		return NULL;
	}

	if (load_pattern(fp, mod) == -1) {
		free(mod);
		return NULL;
	}

	return mod;
}

static int note_str_to_ub(const char *note_str)
{
	int note;

	for (note = 0; note < 12; note++)
		if (strcmp(note_table[note], note_str) == 0)
			return note;

	return -1;
}

int load_header(FILE *fp, mod_t *mod)
{
	uint	bpm;

	// Read bpm
	fscanf(fp, "%u", &bpm);
	assert(bpm > 0 && bpm < 256);
	mod->bpm = (byte) bpm;
	return 0;
}

int load_pattern(FILE *fp, mod_t *mod)
{
	note_t	*note;
	uint	p, c;
	char	note_str[4];
	int		note_ub;
	int		ins_ub;

	// Load
	for (p=0; p<64; p++) {
		if (feof(fp))
			break;

		for (c=0; c<4; c++) {
			note = &(mod->pats[0].chns[c].notes[p]);

			// Read a note
			if (fscanf(fp, "%3s", note_str) == EOF)
				return -1;

			if (note_str[2] == '=')
				note->note = 90;
			else {
				note->note = (note_str[2] - '0') * 12;

				note_str[2] = 0;
				note_ub = note_str_to_ub(note_str);
				if (note_ub == -1) {
					note->note = 0;
					note->freq = 0;
				}
				else {
					note->note += note_ub;

					/*                  12 ___
										1.059463 ~= 1 +  ./ 2
					 */
					note->freq = 1.0;				// Fill the freq.
					note_ub = note->note;
					while (note_ub--)
						note->freq *= 1.059463;
				}
			}

			// Read the instrument
			fscanf(fp, "%x", &ins_ub);
			assert(ins_ub >=0 && ins_ub <= 0xFF);
			note->ins_id = (byte) ins_ub;

			// Read the volumn
			fscanf(fp, "%x", &ins_ub);
			assert(ins_ub >=0 && ins_ub <= 0x40);
			note->vol = (byte) ins_ub;

			// Skip the splite line
			fscanf(fp, "%3s", note_str);
		}
	}
	return 0;
}

