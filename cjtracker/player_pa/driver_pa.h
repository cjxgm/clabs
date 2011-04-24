/** driver_pa.h
 * CJTM Player: Driver (PulseAudio)
 */

#ifndef __CJTMPLAY_DRIVER_PA__
#define __CJTMPLAY_DRIVER_PA__

#include "type.h"

int		sound_init(void);
void	sound_free(void);
void	sound_play(byte snd);
void	sound_wait(void);

#endif
