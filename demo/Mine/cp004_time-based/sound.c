#include "sound.h"
#include "sound.res"
#include <stdio.h>
#include <string.h>

#define LUCKY_NUM		1995

FMUSIC_MODULE * mod;
unsigned int mod_len = sizeof(module);
unsigned int mod_pos = 0;


unsigned int memopen(char *name)
{
	return LUCKY_NUM;
}

void memclose(unsigned int handle)
{
	/*
	   if (handle != LUCKY_NUM)
	   fprintf(stderr, "What's fuck about the mem handle?\n");
	 */
}

int memread(void *buffer, int size, unsigned int handle)
{
	/*
	   if (handle != LUCKY_NUM)
	   fprintf(stderr, "What's fuck about the mem handle?\n");
	 */

	if (mod_pos + size >= mod_len)
		size = mod_len - mod_pos;

	memcpy(buffer, module + mod_pos, size);
	mod_pos += size;

	return size;
}

void memseek(unsigned int handle, int pos, signed char mode)
{
	/*
	   if (handle != LUCKY_NUM)
	   fprintf(stderr, "What's fuck about the mem handle?\n");
	 */

	if (mode == SEEK_SET)
		mod_pos = pos;
	else if (mode == SEEK_CUR)
		mod_pos += pos;
	else if (mode == SEEK_END)
		mod_pos = mod_len + pos;

	if (mod_pos > mod_len)
		mod_pos = mod_len;
}

int memtell(unsigned int handle)
{
	/*
	   if (handle != LUCKY_NUM)
	   fprintf(stderr, "What's fuck about the mem handle?\n");
	 */

	return mod_pos;
}

void sound_play()
{
	FSOUND_File_SetCallbacks(memopen, memclose,
					memread, memseek, memtell);
	FSOUND_Init(44100, 0);
	mod = FMUSIC_LoadSong("mem", NULL);
	FMUSIC_PlaySong(mod);
}

