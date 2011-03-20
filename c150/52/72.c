# include <stdio.h>
# include<graphics.h>
     int main()
     { 
          int i, gdriver, gmode, size; 
          void *buf; 
          gdriver=DETECT; 
          initgraph(&gdriver, &gmode, ""); 
          setbkcolor(BLUE); 
          cleardevice(); 
          setcolor(LIGHTRED); 
          setlinestyle(0,0,1); 
          setfillstyle(1, 10); 
          circle(100, 200, 30); 
          floodfill(100, 200, 12); 
          size=imagesize(69, 169, 131, 231); 
          buf=malloc(size); 
          getimage(69, 169, 131, 231,buf); 
          putimage(500, 269, buf, COPY_PUT); 
          for(i=0; i<185; i++){ 
               putimage(70+i, 170, buf, COPY_PUT); 
               putimage(500-i, 170, buf, COPY_PUT); 
          } 
          for(i=0;i<185; i++){ 
               putimage(255-i, 170, buf, COPY_PUT); 
               putimage(315+i, 170, buf, COPY_PUT); 
          } 
          getch(); 
          closegraph(); 
        }
