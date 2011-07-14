#ifndef TDEC_H
#define TDEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_thread.h"


const char TDEC_INNER_OUTER_FILL = 1;
const char TDEC_OUTER_INNER_FILL = 0;
const char TDEC_FADE_IN = 1;
const char TDEC_FADE_OUT = 0;

#include "fps.h"
#include "video.h"
#include "utils.h"
#include "layers.h"
#include "scroller.h"
#include "list.h"

#ifdef __cplusplus
}
#endif

#endif
