# include "graphics.h"
 # include "math.h"
 # include "conio.h"
 # define PATH "d:\\tc"
 # define PI 3.1415926535
 main()
  {
    int gdriver=VGA;
    int gmode = VGAHI;
    double a=0,b;
    int x0=340,y0=240,radius=100,i,x,y;
    initgraph(&gdriver,&gmode,PATH);
    setcolor(2);
    setlinestyle(0,0,0);
    for(i=0;i<6;i++,a+=60)
      { b=a*PI/180;
	x=x0+radius*cos(b);
	y=y0+radius*sin(b);

	arc(x,y,120-i*60,240-i*60,radius);

	}
      getch();
      closegraph();
      }
