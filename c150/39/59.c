 # include "graphics.h"
 # include "conio.h"
 # define PATH "d:\\tc"
 main()
   { int gdriver=VGA ,gmode=VGAHI;
    unsigned char c[8]={0x0f,0x0f,0x0f,0x0f,0xf0,0xf0,0xf0,0xf0};
   initgraph (&gdriver,&gmode ,PATH);
   setcolor(RED);
  circle(320,240,150);
  setcolor(GREEN);
  rectangle(120,200,520,280);
  setfillpattern(c,YELLOW);
  floodfill(320,240,GREEN);
       getch();
       closegraph();
       }