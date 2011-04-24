/** driver_pa.c
 * CJTM Player: Driver (PulseAudio)
 */

#include <stdio.h>
#include <pulse/simple.h>
#include "driver_pa.h"

static	byte	snds[2048]	= {0};
static	int		pos			= 0;

static	pa_simple		*s	= NULL;
static	pa_sample_spec	 ss	= {
	.format		= PA_SAMPLE_U8,
	.rate		= 8000,
	.channels	= 1
};

int sound_init(void)
{
	int tries = 3;

	while (tries--) {
		s = pa_simple_new(NULL,
						"CJTM Player",
						PA_STREAM_PLAYBACK,
						NULL,
						"Clanjor Tracker Module",
						&ss,
						NULL,
						NULL,
						NULL);
		if (s == NULL) {
			sleep(1);
			continue;
		}

		return 0;
	}

	return -1;
}

void sound_play(byte snd)
{
	int err;

	snds[pos++] = snd;
	if (pos == 2048) {
		pa_simple_write(s,
						snds,
						pos,
						&err);
		pos = 0;
	}

}

void sound_free(void)
{
	if (s) pa_simple_free(s);
}

void sound_wait(void)
{
	int err;

	if (pos != 0) {
		pa_simple_write(s,
						snds,
						pos,
						&err);
	}

	pa_simple_drain(s, &err); // Wait for all datas to be played.
}

