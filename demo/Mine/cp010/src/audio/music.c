
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
#define BPM_TO_MSPR(BPM,RPB)	60000.0f / (BPM) / (RPB)
float music_mspr = BPM_TO_MSPR(120, 2);		// MilliSeconds Per Row
float music_amplify = 1.0f;

// scores
static u16 s1[] = {
	0x4100, 0x3800, 0x4300, 0x3800,
	0x4100, 0x3800, 0x4300, 0x3800,
	0x4100, 0x3800, 0x4300, 0x3800,
	0x4100, 0x3800, 0x4300, 0x3800,
	0x4100, 0x3800, 0x4300, 0x3800,
	0x4100, 0x3800, 0x4300, 0x3800,
	0xFFFF
};

static u16 s2[] = {
	0x3101, 0x2800, 0x2801, 0x2801,
	0x2801, 0x2801, 0x2801, 0x2801,
	0x3101, 0x2801, 0x2801, 0x2801,
	0x2801, 0x2801, 0x2801, 0x2801,
	0x3101, 0x0100, 0x2801, 0x2801,
	0x2801, 0x0100, 0x2801, 0x0100,
	0xFFFF
};

u8    music_nscore = 2;
u16 * music_scores[] = {s1, s2};

// insts
#define INST(NAME)	float NAME (float x)

INST(osc_pulse) { 
	x = mod(x, 2*PI);
	return (x<PI ? -1.0f : 1.0f);
}
INST(osc_tri) {
	x = mod(x, 2*PI);
	if (x < PI) return lerp(x, 0, PI, -1, 1);
	return lerp(x, PI, 2*PI, 1, -1);
}
INST(inst_hit) {
	if (x > 30.0f * 2*PI) return 0.0f;
	static float t;
	t = osc_pulse(x);
	if (x < 10.0f * 2*PI) return t * lerp(x, 0, 10.0f * 2*PI, 1.0f, 0.2f);
	return t * lerp(x, 10.0f * 2*PI, 30.0f * 2*PI, 0.2f, 0.0f);
}
INST(inst_low) {
	return osc_tri(x) * 0.3f;
}

u8 music_ninst = 2;
MusicOsc music_insts[] = {
/* 00 */	inst_hit,
/* 01 */	inst_low,
};

