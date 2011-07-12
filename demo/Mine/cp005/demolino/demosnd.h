/** demosnd.h
 * Demolino: Sound
 **/
#ifndef __DEMOSND_H__
#define __DEMOSND_H__
#include <SDL/SDL.h>

int snd_init(void);
void snd_destroy(void);
extern Uint8 snd_data[2048];
extern int snd_len;

#endif

