/** loader.c
 * CJTM Player: Loader
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "loader.h"

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

/* filename = "*.cjtm.ptf"				*/
/* cjtm:	Clanjor Tracker Module		*/
/* ptf:		Pure Text Format			*/
mod_t *load(const char *filename)
{
	FILE	*fp;
	mod_t	*mod;
	note_t	*note;

	uint	p, c;
	uint	bpm;
	char	note_str[4];
	int		note_ub;
	int		ins_ub;
	
	if ((fp = fopen(filename, "r")) == NULL)
		return NULL;
	
	if ((mod = calloc(sizeof(mod_t), 1)) == NULL) {
		fclose(fp);
		return NULL;
	}

	// Read bpm
	fscanf(fp, "%u", &bpm);
	assert(bpm > 0 && bpm < 256);
	mod->bpm = (byte) bpm;

	// Read notes
	for (p=0; p<64; p++) {
		if (feof(fp))
			break;

		for (c=0; c<4; c++) {
			note = &(mod->pats[0].chns[c].notes[p]);

			// Read a note
			if (fscanf(fp, "%3s", note_str) == EOF)
				goto _end_of_reading;

			if (note_str[2] == '=')
				note->note = 90;
			else {
				note->note = (note_str[2] - '0') * 12;

				note_str[2] = 0;
				note_ub = note_str_to_ub(note_str);
				if (note_ub == -1)
					note->note = 0;
				else {
					note->note += note_ub;

					/*                  12 ___
										1.059463 ~= 1 +  ./ 2
					 */
					note->freq = 1.0;
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
	_end_of_reading:

	fclose(fp);
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

