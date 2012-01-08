
#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "link.h"
#include <stdio.h>

typedef struct Obj
{
	Link face;
}
Obj;

typedef struct Vert
{
	float pos[3];
}
Vert;

typedef struct LinkFace
{
	struct LinkFace * prev, * next;
	Link vert;
}
LinkFace;

typedef struct LinkVert
{
	struct LinkVert * prev, * next;
	Vert * v;
}
LinkVert;

Obj * load_obj(FILE * fp);
void draw_obj(Obj * o);

#endif

