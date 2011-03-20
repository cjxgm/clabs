# include "graphics.h"
# include "stdlib.h"
# include "conio.h"
# define PATH "d:\\tc"
main()
 { int gdriver=VGA;
   int  gmode=VGAHI;
    initgraph (&gdriver,&gmode,PATH);
    printf("press anykey to stop.");
    while(!kbhit())
    putpixel(rand()%640,rand()%440+40,rand()%15+1);
    getch();
    getch();
    closegraph();
    }