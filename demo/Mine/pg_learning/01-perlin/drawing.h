
#ifndef __DRAWING_H
#define __DRAWING_H

#include <FL/fl_draw.H>

#define setcolor(R,G,B)				fl_color((R), (G), (B))
#define putpixel(X,Y)				fl_point((X), (Y))
#define drawline(X0,Y0,X1,Y1)		fl_line((X0), (Y0), (X1), (Y1))

#endif

