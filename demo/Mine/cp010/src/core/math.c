
/************************************************************
 * cp: math.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include "math.h"
#include "core.h"

float lerp(float v, float vf, float vt, float df, float dt)
{
	return (v-vf) / (vt-vf) * (dt-df) + df;
}

float mod(float a, float b)
{
	return a - ((s32)(a/b)) * b;
}

