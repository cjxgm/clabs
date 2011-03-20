# include "graphics.h"
# include "conio.h"
# include "stdlib.h"
# define  PATH "d:\\tc"
main()
 { int i,j,imsize;
   int x=60,y=68,k=2;
   char *buffer1,*buffer2;
   int gdriver=VGA,gmode=VGAHI ;
   initgraph(&gdriver,&gmode,PATH);
   setfillstyle(1,GREEN);
   bar(70,70,120,100);
   bar(120,84,150,100);
   setcolor(EGA_BROWN);
   setfillstyle(1,EGA_BROWN);
   circle(84,103,8);
   floodfill(84,103,EGA_BROWN);
   circle(134,103,8);
   floodfill(134,103,EGA_BROWN);
   bar(20,122,556,142);
   bar(556,60,600,142);
   imsize=imagesize(x,y,152,112);
   buffer1=(char*)malloc(imsize);
   buffer2=(char*)malloc(imsize);
   getimage(x,y,152,112,buffer1);
   while(!kbhit())
    { for(i=0;i<=400;i+=k)
      putimage(x+i,y,buffer1,COPY_PUT);
      j=i-k+x;
      for(i=0;i<152-x;i+=k)
       { imsize=imagesize(j+i,y,152-x+j,112);
	 buffer2=(char*)realloc(buffer2,imsize);
	 getimage(j+i,y,152-x+j-k,112,buffer2);
	 putimage(j+i+k,y,buffer2,COPY_PUT);
	 }
       }
       getch();
       free(buffer1);
       free(buffer2);
       closegraph();
       }