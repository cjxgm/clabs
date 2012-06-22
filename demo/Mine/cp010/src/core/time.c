
/************************************************************
 * cp: time.c
 *
 * Licensed under GPLv2.
 * ABSOLUTELY NO WARRANTY!
 *
 * Copyright (C) eXerigumo Clanjor (哆啦比猫/兰威举).
 ************************************************************/

#include "time.h"
#include <time.h>

static float time_begin;
static float ellapsed();




float ticks()
{
	return ellapsed() - time_begin;
}

void time_adjust(float time_should_be)
{
	time_begin += ticks() - time_should_be;
}




static float ellapsed()
{
	return clock() * 1000.0f / CLOCKS_PER_SEC;
}

