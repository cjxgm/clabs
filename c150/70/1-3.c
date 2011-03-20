/*1-3.c*/
     #include "stdio.h"
     #include "graphics.h"
     main()
     {
          int gdriver, gmode, i,j;
          gdriver=DETECT;
          initgraph(&gdriver, &gmode, ""); /*图形初始化*/
          setbkcolor(0);                   /*设置图形背景*/ 
          cleardevice(); 
          for(i=0; i<=15; i++) 
          { 
            setcolor(i);                   /*设置不同作图色*/ 
            circle(319, 239, 20+i*15);     /*画半径不同的圆*/ 
            delay(3000);                   /*延迟3000毫秒*/ 
          } 
          for(i=0; i<=15; i++) 
          { 
             setbkcolor(i);                /*设置不同背景色*/ 
             cleardevice(); 
       for(j=0; j<=15;j++) 
          { 
            setcolor(j);                   /*设置不同作图色*/ 
            circle(319, 239, 20+j*15);     /*画半径不同的圆*/ 
            delay(3000); 
             } 
          } 
      getch();
      closegraph(); 
      } 
