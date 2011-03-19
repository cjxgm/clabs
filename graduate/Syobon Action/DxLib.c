#include <stdlib.h>
#include <stdarg.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <assert.h>
#include "DxLib.h"

static SDL_Surface *screen = NULL;;
static byte trans_r = 0xFF;
static byte trans_g = 0xFF;
static byte trans_b = 0xFF;
static TTF_Font *font = NULL;
static int font_type = DX_FONTTYPE_NORMAL;
#define MAX_SOUND 30
static Mix_Chunk *sounds[MAX_SOUND] = { NULL };
static int free_sound = 0;
static Uint8 *key_buf;

int DxLib_Init(string title, int w, int h, int bitdepth)
{
	atexit(SDL_Quit);
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return -1;
	screen = SDL_SetVideoMode(w, h,
								bitdepth, SDL_HWSURFACE);
	if (screen == NULL) return -1;
	SDL_WM_SetCaption(title, "");

	if (TTF_Init() != 0) return -1;
	if (Mix_OpenAudio(44100, AUDIO_S16, 2, 4096))
		return -1;
	key_buf = SDL_GetKeyState(NULL);
}

inline void DxLib_End()
{
	TTF_CloseFont(font);
	Mix_AllocateChannels(0);
	Mix_CloseAudio();
	SDL_Quit();
}

int ProcessMessage()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		switch (event.type){
		case SDL_QUIT:
			return 1;
		default:
			break;
		}
	}
	return 0;
}

inline void WaitTimer(int interval)
{
	SDL_Delay(interval);
}

inline void ScreenFlip()
{
	SDL_Flip(screen);
}

void DrawGraph(int x, int y, int graphid, int no_use)
{
	SDL_Surface *graph = (SDL_Surface*)graphid;
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(graph, NULL, screen, &offset);
}

inline int GetNowCount()
{
	return SDL_GetTicks();
}

inline int GetRand(int max)
{
	srand(GetNowCount());
	return (double)rand()/RAND_MAX*max;
}

void WaitKey()
{
	SDL_Event event;

	for(;;){
		if (SDL_PollEvent(&event)){
			if (event.type == SDL_KEYDOWN)
				break;
			else if (event.type == SDL_QUIT){
				DxLib_End();
				exit(0);
			}
		}
	}
}

inline int CheckHitKey(int key) // return 1=>down; 0=>not down
{
	return key_buf[key];
}


inline void ClearDrawScreen(){
	SDL_FillRect(screen, &screen->clip_rect,
			SDL_MapRGB(screen->format, 0, 0, 0));
}

inline uint GetColor(byte r, byte g, byte b)
{
	return r*256*256*256 + g*256*256 + b*256 + 255;
}

inline void SetFontSize(int size)
{
	if (font != NULL) TTF_CloseFont(font);
	font = TTF_OpenFont("font.ttf", size);
	assert(font != NULL);
}

void DrawFormatString(int x, int y,
					uint color, string fmt, ...)
{
	assert(font != NULL);

	// Format string
	char msg[256];
	va_list ap;
	va_start(ap, fmt);
	vsprintf(msg, fmt, ap);
	va_end(ap);

	// Convert color
	SDL_Color clr;
	color /= 256;
	clr.b = color%256;
	color /= 256;
	clr.g = color%256;
	color /= 256;
	clr.r = color%256;

	// Draw text
	SDL_Surface *graph;
	switch(font_type){
	case DX_FONTTYPE_NORMAL:
		graph = TTF_RenderUTF8_Blended(font, msg, clr);
		break;
	case DX_FONTTYPE_EDGE:
	// TODO Finish this
		graph = TTF_RenderUTF8_Blended(font, msg, clr);
		break;
	default:
		assert("Font type error!"[0]==0);
	}
	DrawGraph(x, y, (int)graph, 0);
	SDL_FreeSurface(graph);
}

void DrawRotaGraph(int x, int y, float rate, // no use?
				float angle, int graphid, int nouse)
{
	SDL_Surface *graph = (SDL_Surface*)graphid;
	SDL_Surface *graph2 = rotozoomSurface(graph, 
				angle*180/3.1415926, 1.0, false);
	DrawGraph(x, y, (int)graph2, 0);
	SDL_FreeSurface(graph2);
}

inline void ChangeFontType(int fonttype)
{
	font_type = fonttype;
}

inline void StopSoundMem(int soundid)
{
	Mix_HaltChannel(soundid);
}

inline void PlaySoundMem(int soundid, int playtype)
{
	Mix_PlayChannel(soundid, sounds[soundid], playtype);
}

inline void DrawPixel(int x, int y, uint color)
{
	pixelColor(screen, x, y, color);
}

inline void DrawLine(int x0, int y0,
					int x1, int y1, uint color)
{
	lineColor(screen, x0, y0, x1, y1, color);
}

inline void DrawBox(int x0, int y0,
			int x1, int y1, uint color, int is_fill)
{
	if (is_fill)
		boxColor(screen, x0, y0, x1, y1, color);
	else
		rectangleColor(screen, x0, y0, x1, y1, color);
}

void DrawOval(int x, int y, int rx, 
			int ry, uint color, int is_fill) // 椭圆
{
	if (is_fill)
		filledEllipseColor(screen, x, y, rx, ry, color);
	else
		ellipseColor(screen, x, y, rx, ry, color);
}

void DrawTurnGraph(int x, int y,
			int graphid, int nouse) // 画面左右反转
{
	SDL_Surface *graph = (SDL_Surface*)graphid;
	SDL_Surface *graph2 = zoomSurface(graph, 
							-1.0, 1.0, false);
	DrawGraph(x, y, (int)graph2, 0);
	SDL_FreeSurface(graph2);
}

inline void DrawString(int x, int y, 
			string str, uint color) // EdgeColor = 0;
{
	DrawFormatString(x, y, color, "%s", str);
}

inline int CheckSoundMem(int soundid) // 0: stopped; 1: playing
{
	return Mix_Playing(soundid);
}

/************ loadg.c ****************/

inline void SetTransColor(byte r, byte g, byte b)
{
	trans_r = r;
	trans_g = g;
	trans_b = b;
}

int LoadGraph(string filename)
{
	// Load
	SDL_Surface *graph = IMG_Load(filename);
	assert(graph != NULL);

	// Optimize
	SDL_Surface *graph2 = SDL_DisplayFormat(graph);
	SDL_FreeSurface(graph);
	assert(graph2 != NULL);

	return (int)graph2;
}

int DerivationGraph(int x, int y, int w, int h, int graphid)
{
	SDL_Surface *graph = (SDL_Surface*) graphid;
	SDL_Surface *graph2 = SDL_CreateRGBSurface(
				SDL_HWSURFACE, w, h, graph->format->BitsPerPixel, 0, 0, 0, 0);

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	SDL_BlitSurface(graph, &rect, graph2, &graph2->clip_rect);

	// Transparent
	SDL_SetColorKey(graph2, SDL_SRCCOLORKEY,
			SDL_MapRGB(graph2->format, trans_r, trans_g, trans_b));

	return (int)graph2;
}

void GetGraphSize(int graphid, int *w, int *h)
{
	if (graphid == 0){
		*w = 0;
		*h = 0;
		return;
	}
	SDL_Surface *graph = (SDL_Surface*)graphid;
	*w = graph->clip_rect.w;
	*h = graph->clip_rect.h;
}


int LoadSoundMem(string filename)
{
	sounds[free_sound] = Mix_LoadWAV(filename);
	assert(sounds[free_sound] != NULL);
	Mix_AllocateChannels(free_sound+1);
	return free_sound++;
}

inline void ChangeVolumeSoundMem(int vol, int soundid)
{
	Mix_Volume(soundid, vol);
}

