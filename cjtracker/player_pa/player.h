/** player.h
 * CJTM Player: Player
 */

#ifndef __CJTMPLAY_PLAYER__
#define __CJTMPLAY_PLAYER__

#include "type.h"

extern byte		snd_data;

int player_init(void);
void player_free(void);
void play(mod_t *mod);

#endif
