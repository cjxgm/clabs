/** cjtmplay.c
 * Clanjor Tracker Module Player
 */

#include <stdio.h>
#include "type.h"
#include "instrument.h"
#include "player.h"
#include "loader.h"

#define ALIGN_SIZE		6

int main(int argc, char *argv[])
{
	const char *filename = "test.cjtm.ptf";
	if (argc == 2)
		filename = argv[1];
	else if (argc > 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return -1;
	}
	
	mod_t *mod = load(filename);
	if (mod == NULL) {
		fprintf(stderr, "Cannot load '%s'!\n", filename);
		return -1;
	}

	/********** DEBUG BEGIN **********/
	printf("[ Module:\n");
	printf("\t%-*s= %u\n", ALIGN_SIZE, "bpm", (uint) mod->bpm);

	int p;
	for (p=0; p<1; p++) {
		printf("\t[ Pattern (%2.2i):\n", p);
		pat_t *pat = &mod->pats[p];

		int c;
		for (c=0; c<4; c++) {
			printf("\t\t[ Channel (%2.2i):\n", c);
			chn_t *chn = &pat->chns[c];

			int n;
			printf("\t\t\t");
			for (n=0; n<64; n++) {
				printf("%2.2u  ", (uint) chn->notes[n].note);
				if (n != 0 && (n+1) % 16 == 0 && n != 63)
					printf("\n\t\t\t");
			}

			printf("\n\t\t]\n");
		}

		printf("\t]\n");
	}

	printf("]\n");
	fflush(stdout);
	/********** DEBUG  END  **********/

	if (player_init() == -1) {
		fprintf(stderr, "Cannot open /dev/dsp for writing!\n");
		return -1;
	}
	play(mod);
	player_free();

	return 0;
}

