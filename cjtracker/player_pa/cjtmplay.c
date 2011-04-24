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
	
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Unable to open file '%s'.\n", filename);
		return -1;
	}

	play_mod_file(fp);

	fclose(fp);
	return 0;
}

