/*10-3.c*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
int color=4,i;
int start_x,end_x,start_y,end_y;
int X[480],Y[640];
void hor_wave_line(int x0,int y1,int x1,int a,int k);
void ver_wave_line(int x0,int y0,int y1,int a,int k);
main( )
{
  int driver=EGA,mode=EGAHI;
  initgraph(&driver,&mode,"");
  cleardevice();
  start_x=200;
  start_y=50;
  end_x=450;
  end_y=200;
  setcolor(4);
hor_wave_line(start_x,start_y,end_x,0,2);
ver_wave_line(end_x,start_y,end_y,10,3);
hor_wave_line(start_x,end_y,end_x,0,2);
line(start_x,start_y-2,start_x,end_y+5);
setfillstyle(1,14);
bar3d(start_x,start_y-5,start_x,end_y+150,2,1);
setfillstyle(1,4);
floodfill(start_x+50,start_y+50,4);
setactivepage(1);
hor_wave_line(start_x,start_y,end_x,0,4);
ver_wave_line(end_x,start_y,end_y,10,6);
hor_wave_line(start_x,end_y,end_x,0,4);
line(start_x,start_y-2,start_x,end_y+5);
setfillstyle(1,14);
bar3d(start_x,start_y-5,start_x,end_y+150,2,1);
setfillstyle(1,4);
floodfill(start_x+50,start_y+50,4);
while(! kbhit( )){
setvisualpage(1);
delay(200);
setvisualpage(0);
delay(200);
}
closegraph( );
}
void hor_wave_line(int x0,int y0,int x1,int a,int k)
{
float rad=3.1415926/180.0;
for(i=0;i<640;i++);
   Y[i]=random(1)+(int)(5*sin((i*k+a)*rad));
for(i=x0;i<x1;i++)
{
  moveto(i,Y[i]+y0);
  lineto(i,Y[i]+y0);
}
lineto(x1,y0);
}
void ver_wave_line(int x0,int y0,int y1,int a,int k)
{
float rad=3.1415926/180.0;
randomize( );
for(i=0;i<480;i++)
   X[i]=random(1)+(int)(5*sin((i*k+a)*rad));
for(i=y0+1;i<y1;i++)
{
  lineto(X[i]+x0,i);
  moveto(X[i]+x0,i);
}
lineto(x0,y1);
}
