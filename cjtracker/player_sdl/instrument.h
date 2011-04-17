/** instrument.h
 * CJTM Player: Instrument
 */

#ifndef __CJTMPLAY_INSTRUMENT__
#define __CJTMPLAY_INSTRUMENT__

#include "type.h"

// vol <- [0, 64]
void mix_instrument(byte ins_id, float angle, byte vol);

#endif
