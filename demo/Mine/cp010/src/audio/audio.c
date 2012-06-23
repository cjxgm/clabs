
/************************************************************
 * cp: audio.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include "audio.h"
#include "seq.h"
#include "music.h"
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define BUF_SIZE	512

u32 audio_bitrate = 8000;
u8  audio_playing = 0;

static FILE * dsp;
static u8 buf[BUF_SIZE];

static pthread_t thread_play;
static void play_thread();
inline static void fillbuf();




void audio_open()
{
	dsp = fopen("/dev/dsp", "w");
	assert(dsp && "Can not open /dev/dsp");
}

void audio_close()
{
	audio_stop();
	fclose(dsp);
}

void audio_play()
{
	if (audio_playing) return;

	audio_playing = 1;
	seq_init();
	music_init();
	pthread_create(&thread_play, PTHREAD_CREATE_JOINABLE,
			(void *)&play_thread, NULL);
}

void audio_stop()
{
	void * retval;

	if (audio_playing) {
		audio_playing = 0;
		pthread_join(thread_play, &retval);
	}
}




static void play_thread()
{
	float time_start = ticks();
	u32   played     = 0;

	while (audio_playing) {
		fillbuf();
		fwrite(buf, sizeof(buf), 1, dsp);
		fflush(dsp);
		time_adjust(time_start + 1000.0f*(++played) *
				BUF_SIZE / audio_bitrate);
	}
}

inline static void fillbuf()
{
	float s = 1000.0f / audio_bitrate;
	float t = ticks();
	int i;
	for (i=0; i<BUF_SIZE; i++)
		buf[i] = (u8)lerp(seq_mix_all(t + s*i), -1, 1, 0, 0xFF);
}

