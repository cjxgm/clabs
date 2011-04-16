/** instrument.c
 * CJTM Player: Instrument
 */

#include <math.h>
#include "instrument.h"
#include "player.h"

#define MIX_WAY		1

#if (MIX_WAY == 0)
#define MIX(X, Y) (((X) + (Y)) / 2)
#else
#define MIX(X, Y) (((X)*(X) + (Y)*(Y)) / ((X) + (Y)))
#endif

#ifndef PI
#define PI 3.1415927
#endif

static double saw(double angle);

void mix_instrument(byte ins_id, float angle, byte vol)
{
	float value;
	byte snd;

	switch (ins_id) {
		default:
		case 0:
			value = sin(angle);
			snd = (value + 1) / 2 * 255;
			break;
		case 1:
			value = saw(angle);
			snd = value * 32 + 127;
			break;
		case 2:
			value = (rand() % 256) / 255.0;
			snd = value * 64 + 127;
			break;
	}

#if (MIX_WAY == 0)
		snd_data = MIX(snd_data - 127, vol * snd / 64 - 127) + 127;
#else
	byte m1, m2;
	m1 = snd_data;
	m2 = vol * snd / 64;

	if (m1 + m2 != 0)
		snd_data = MIX(m1, m2);
#endif

}

double saw(double angle)
{
	while (angle >= 2.0 * PI) angle -= 2.0 * PI;
	if (angle >= PI) return -saw(angle - PI);
	if (angle >= PI / 2.0) angle = PI - angle;
	return angle / (PI / 2.0 ) * 2.0 - 1.0;
}

