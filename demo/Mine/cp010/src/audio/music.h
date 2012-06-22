
/************************************************************
 * cp: music.h
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#ifndef __MUSIC_H
#define __MUSIC_H

#include "../core/core.h"

extern u8    music_nscore;
extern u16 * music_scores[];

typedef float (* MusicOsc)(float x);
extern u8 music_ninst;
extern MusicOsc music_insts[];

extern float music_mspr;	// MilliSeconds Per Row
extern float music_amplify;

#endif

