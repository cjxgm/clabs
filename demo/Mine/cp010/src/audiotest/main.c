
/************************************************************
 * cp: main.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include <stdio.h>
#include "../core/core.h"
#include "../audio/audio.h"

int main()
{
	audio_open();
	time_adjust(0.0f);
	audio_play();

	while (audio_playing) {}

	audio_close();

	return 0;
}

