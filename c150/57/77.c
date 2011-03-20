# include "graphics.h"
# include "conio.h"
# define  PATH "d:\\tc"
main()
 {  int gdriver=VGA;
    int gmode = VGAHI;
    int i=0,k=0, x=160, y=120;
    char ch[50];
    initgraph(&gdriver,&gmode,PATH);
    while(!kbhit())
     {setviewport(0,0,319,239,1);
      setcolor(i%15+1);
      circle(x,y,i);
      setcolor(0);
      circle(x,y,i-1);
      setviewport(320,240,639,479,1);
      setcolor(15-i%15);
      rectangle(x-i,y-i,x+i,y+i);
      setcolor(0);
      rectangle(x-i+1,y-i+1,x+i-1,y+i-1);
      i++;
      if(i>200)
	{i=0;
	 setcolor(EGA_MAGENTA);
	 setviewport(0,240,320,479,1);
	 clearviewport();
	 sprintf(ch,"number is %d",++k);
	 outtextxy(30,30,ch);
	 }

      }
     getch();
     closegraph();
     }