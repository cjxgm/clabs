// ===========================================================================
// MiniFMOD test player
// Copyright (c), Firelight Multimedia, 2000.
// ===========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __WIN32__
#include <conio.h>
#include <windows.h>
#endif

#include "minifmod.h"

//#define USEMEMLOAD
//#define USEMEMLOADRESOURCE
//#define USESAMPLELOADCALLBACK
#define USEFMOD 

// this is if you want to replace the samples with your own (in case you have compressed them)
#ifdef USESAMPLELOADCALLBACK

void sampleloadcallback(void *buff, int lenbytes, int numbits, int instno, int sampno)
{
  printf("pointer = %p length = %d bits = %d instrument %d sample %d\n", buff, lenbytes, numbits, instno, sampno);
}

#endif

unsigned int fileopen(char *name);

#ifndef USEMEMLOAD

unsigned int fileopen(char *name)
{
  return((unsigned int)fopen(name, "rb"));
}

void fileclose(unsigned int handle)
{
  fclose((FILE *)handle);
}

int fileread(void *buffer, int size, unsigned int handle)
{
  return(fread(buffer, 1, size, (FILE *)handle));
}

void fileseek(unsigned int handle, int pos, signed char mode)
{
  fseek((FILE *)handle, pos, mode);
}

int filetell(unsigned int handle)
{
  return(ftell((FILE *)handle));
}

#else

typedef struct
{
  int length;
  int pos;
  void *data;
}
MEMFILE;

unsigned int memopen(char *name)
{
  MEMFILE *memfile;
  memfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);
#ifndef USEMEMLOADRESOURCE
  { // load an external file and read it
    FILE *fp;
    fp = fopen(name, "rb");
    if (fp)
    {
      fseek(fp, 0, SEEK_END);
      memfile->length = ftell(fp);
      memfile->data = calloc(memfile->length,1);
      memfile->pos = 0;

      fseek(fp, 0, SEEK_SET);
      fread(memfile->data, 1, memfile->length, fp);
      fclose(fp);
    }
  }
#else
  { // hey look some load from resource code!
    HRSRC rec;
    HGLOBAL handle;

    rec = FindResource(NULL, name, RT_RCDATA);
    handle = LoadResource(NULL, rec);

    memfile->data = LockResource(handle);
    memfile->length = SizeofResource(NULL, rec);
    memfile->pos = 0;
  }
#endif
  return((unsigned int)memfile);
}

void memclose(unsigned int handle)
{
  MEMFILE *memfile = (MEMFILE *)handle;
#ifndef USEMEMLOADRESOURCE
  free(memfile->data); // dont free it if it was initialized with LockResource
#endif
  free(memfile);
}

int memread(void *buffer, int size, unsigned int handle)
{
  MEMFILE *memfile = (MEMFILE *)handle;

  if (memfile->pos + size >= memfile->length)
    size = memfile->length - memfile->pos;

  memcpy(buffer, (char *)memfile->data+memfile->pos, size);
  memfile->pos += size;

  return(size);
}

void memseek(unsigned int handle, int pos, signed char mode)
{
  MEMFILE *memfile = (MEMFILE *)handle;

  if (mode == SEEK_SET)
    memfile->pos = pos;
  else if (mode == SEEK_CUR)
    memfile->pos += pos;
  else if (mode == SEEK_END)
    memfile->pos = memfile->length + pos;

  if (memfile->pos > memfile->length)
    memfile->pos = memfile->length;
}

int memtell(unsigned int handle)
{
  MEMFILE *memfile = (MEMFILE *)handle;
  return(memfile->pos);
}

#endif

int main(int argc, char *argv[])
{
#ifdef USEFMOD
  FMUSIC_MODULE *mod;
#ifndef USEMEMLOAD
  FSOUND_File_SetCallbacks(fileopen, fileclose, fileread, fileseek, filetell);
#else
  FSOUND_File_SetCallbacks(memopen, memclose, memread, memseek, memtell);
#endif
#endif

  printf("-------------------------------------------------------------\n");
  printf("MiniFMOD example XM player.\n");
  printf("Copyright (c) Firelight Multimedia, 2000.\n");
  printf("-------------------------------------------------------------\n");

  if (argc < 2)
  {
    printf("Syntax: test infile.xm\n\n");
    return(1);
  }

#ifdef USEFMOD

  // Initialize
  if (!FSOUND_Init(44100, 0))
  {
    printf("Error upon initialization.\n");
    return(1);
  }

  // Load song
#ifdef USESAMPLELOADCALLBACK
  mod = FMUSIC_LoadSong(argv[1], sampleloadcallback);
#else
  mod = FMUSIC_LoadSong(argv[1], NULL);
#endif
  if (!mod)
  {
    FSOUND_Close();
    printf("Error loading song\n");
    return(1);
  }

  // Play song
  FMUSIC_PlaySong(mod);

#endif

#ifdef __LINUX__
  printf("Press enter to quit\n");
#endif
#ifdef __WIN32__
  printf("Press any key to quit\n");
#endif
  printf("-------------------------------------------------------------\n");
  printf("Playing song ...\n");

#ifdef __LINUX__
  getchar();
#endif
#ifdef __WIN32__
  {
    char key = 0;
    do
    {
      int ord = 0, row = 0;
      float mytime = 0;
      if (kbhit())
      {
        key = getch();
      }
      ord = FMUSIC_GetOrder(mod);
      row = FMUSIC_GetRow(mod);
      mytime = (float)FMUSIC_GetTime(mod) / 1000.0f;
      printf("ord %2d row %2d seconds %5.02f %s      \r", ord, row, mytime, (row % 8 ? "    " : "TICK"));
    }
    while (key != 27);
  }
  printf("\n");
#endif

#ifdef USEFMOD
  FMUSIC_StopSong(mod);
  FMUSIC_FreeSong(mod);
  FSOUND_Close();
#endif

  return(0);
}
