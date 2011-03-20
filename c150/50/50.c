#include <graphics.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <conio.h> 
int main(void) 
{ 
   /* request auto detection */ 
   int gdriver = DETECT, gmode, errorcode; 
   int i, maxx, maxy; 
   /* our polygon array */ 
   int poly[8]; 
   /* initialize graphics, local variables */ 
   initgraph(&gdriver, &gmode, ""); 
   /* read result of initialization */ 
   errorcode = graphresult(); 
   if (errorcode != grOk) 
   /* an error occurred */ 
   { 
      printf("Graphics error: %s\n", 
             grapherrormsg(errorcode)); 
      printf("Press any key to halt:"); 
      getch(); 
      exit(1); 
      /* terminate with an error code */ 
   } 
   maxx = getmaxx(); 
   maxy = getmaxy(); 
   poly[0] = 20;        /* 1st vertext */ 
   poly[1] = maxy / 2; 
   poly[2] = maxx - 20; /* 2nd */ 
   poly[3] = 20; 
   poly[4] = maxx - 50; /* 3rd */ 
   poly[5] = maxy - 20; 
   /* 
      4th vertex. fillpoly automatically 
      closes the polygon. 
   */ 
   poly[6] = maxx / 2; 
   poly[7] = maxy / 2; 
   /* loop through the fill patterns */ 
   for (i=EMPTY_FILL; i<USER_FILL; i++) 
   { 
      /* set fill pattern */ 
      setfillstyle(i, getmaxcolor()); 
      /* draw a filled polygon */ 
      fillpoly(4, poly); 
      getch(); 
   } 
   /* clean up */ 
   closegraph(); 
   return 0; 

}