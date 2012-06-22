
/************************************************************
 * cp: music.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include "music.h"
#include <math.h>

// info
float music_mspr = 300.0f;	// MilliSeconds Per Row
float music_amplify = 1.0f;

// scores
static u16 s1[] = {
	0x4100, 0x3A00, 0x4300, 0x3A00,
	0x4100, 0x3A00, 0x4300, 0x3A00,
	0xFFFF
};

static u16 s2[] = {
	0x4101, 0x0000, 0x0000, 0x0000,
	0x4301, 0x0000, 0x0000, 0x0000,
	0xFFFF
};

u8    music_nscore = 2;
u16 * music_scores[] = {s2, s1};

// insts
#define INST(NAME)	float NAME (float x)

INST(osc_sine) { return sin(x); }
INST(osc_tri) {
	x = mod(x, 2*PI);
	if (x < PI) return lerp(x, 0, PI, -1, 1);
	return lerp(x, PI, 2*PI, 1, -1);
}

u8 music_ninst = 2;
MusicOsc music_insts[] = {osc_sine, osc_tri};

