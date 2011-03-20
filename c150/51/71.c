#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <math.h>
main()
{void * ptr;
int gd=DETECT,gm;
float i;
initgraph(&gd,&gm,"d:\\tc");
setactivepage(1);
setcolor(RED);
setfillstyle(1,14);
fillellipse(10,10,10,10);
ptr=malloc(imagesize(0,0,20,20));
getimage(0,0,20,20,ptr);
randomize();
setactivepage(0);
for(i=0;i<3000;i++)
putpixel(random(640),random(480),random(15));
for(i=0;i<6.28*4;i=i+0.005)
{putimage(300+8*(cos(i)+i*sin(i)),240+6*(sin(i)-i*cos(i)),ptr,COPY_PUT);
putimage(300+8*(cos(i)+(i)*sin(i)),240+6*(sin(i)-(i)*cos(i)),ptr,COPY_PUT);
if(kbhit()) break;
delay(10);
}
getch();
closegraph();
}

