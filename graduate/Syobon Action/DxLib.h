#ifndef __DXLIB_H__
#define __DXLIB_H__

#include <stdlib.h>
#include <SDL/SDL.h> 

typedef const char* string;
typedef unsigned char byte;
typedef byte bool;
typedef unsigned int uint;

/****** From loadg.c ******/
void SetTransColor(byte r, byte g, byte b);

// @return	Graph ID
int LoadGraph(string filename);

// @param	An axis of a rectangle
//				从graphid中指定的矩形区域中分离出来
// @return	Graph ID
int DerivationGraph(int x, int y, int w, int h, int graphid);
void GetGraphSize(int graphid, int *w, int *h);
#define SetCreateSoundDataType(type)

// @return	Sound ID
int LoadSoundMem(string filename);

// @param	vol: volume ranged from 0~100
// @param	soundid: Sound ID
void ChangeVolumeSoundMem(int vol, int soundid);

/****** From main.c ******/
int DxLib_Init(string title, int w, int h, int bitdepth);
void DxLib_End();
int ProcessMessage();
int CheckHitKey(int key); // return 1=>pressed; 0=>unpressed
void WaitKey();
void SetFontSize(int size);
#define SetFontThickness(...)
// void SetFontThickness(int thickness);
#define SetDrawScreen(...)
void ClearDrawScreen();
void DrawFormatString(int x, int y, uint color, string fmt, ...);
uint GetColor(byte r, byte g, byte b);
void DrawRotaGraph(int x, int y, float rate, float angle, int graphid, int is_trans);
void ChangeFontType(int fonttype);
void ScreenFlip();
int GetNowCount();
void StopSoundMem(int soundid);
void PlaySoundMem(int soundid, int playtype);
void WaitTimer(int interval);
int GetRand(int max);
void DrawPixel(int x, int y, uint color);
void DrawLine(int x0, int y0, int x1, int y1, uint color);
void DrawBox(int x0, int y0, int x1, int y1, uint color, int is_fill);
void DrawOval(int x, int y, int rx, int ry, uint color, int is_fill); // 椭圆
void DrawGraph(int x, int y, int graphid, int is_trans);
void DrawTurnGraph(int x, int y, int graphid, int is_trans); // 画面左右反转
void DrawString(int x, int y, string str, uint color); // EdgeColor = 0;
int CheckSoundMem(int soundid); // 0: stopped; 1: playing
#define TRUE 1
#define FALSE 0

#define DX_FONTTYPE_NORMAL 0
#define DX_FONTTYPE_EDGE 1

#define DX_PLAYTYPE_BACK 0
#define DX_PLAYTYPE_LOOP -1

#define DX_SCREEN_BACK 0

#define KEY_INPUT_0 SDLK_0
#define KEY_INPUT_1 SDLK_1
#define KEY_INPUT_2 SDLK_2
#define KEY_INPUT_3 SDLK_3
#define KEY_INPUT_4 SDLK_4
#define KEY_INPUT_5 SDLK_5
#define KEY_INPUT_6 SDLK_6
#define KEY_INPUT_7 SDLK_7
#define KEY_INPUT_8 SDLK_8
#define KEY_INPUT_ESCAPE SDLK_ESCAPE
#define KEY_INPUT_F1 SDLK_F1
#define KEY_INPUT_F2 SDLK_F2
#define KEY_INPUT_O SDLK_o
#define KEY_INPUT_RETURN SDLK_RETURN
#define KEY_INPUT_SPACE SDLK_SPACE
#define KEY_INPUT_Z SDLK_z
#define PAD_INPUT_DOWN SDLK_DOWN
#define PAD_INPUT_LEFT SDLK_LEFT
#define PAD_INPUT_RIGHT SDLK_RIGHT
#define PAD_INPUT_UP SDLK_UP

/****** DxLib.c ******/
#define true TRUE
#define false FALSE

#endif

