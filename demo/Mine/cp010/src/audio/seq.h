
/************************************************************
 * cp: seq.h
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#ifndef __SEQ_H
#define __SEQ_H

#include "../core/core.h"

typedef struct Sequence
{
	u16 * score;
	u32   instpos;
	float last_time;
	float w;
	u16   notepos;
	u8    inst;
}
Sequence;


void seq_init();
float seq_mix_all(float time);
float seq_pull(Sequence * seq, float time);
const Sequence * seq_get_all();

#endif

