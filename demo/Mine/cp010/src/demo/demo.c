
/************************************************************
 * cp: demo.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include <stdio.h>
#include "../core/core.h"
#include "../audio/audio.h"
#include "../video/video.h"

int main()
{
	video_init();
	return 0;
}

void demo_init()
{
	audio_open();
	time_adjust(0.0f);
	audio_play();
}

void demo_close()
{
	printf("Destroying...\n");
	audio_close();
}

