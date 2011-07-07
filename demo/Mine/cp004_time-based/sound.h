
#ifndef __SOUND__
#define __SOUND__

#include "minifmod.h"

extern FMUSIC_MODULE * mod;
void sound_play();

#define sound_get_time()		FMUSIC_GetTime(mod)
#define sound_get_row()			FMUSIC_GetRow(mod)
#define sound_get_order()		FMUSIC_GetOrder(mod)

#endif
