# include "graphics.h"
# include "conio.h"
# define PATH "d:\\tc"
main()
 { int i,gdriver=VGA,
   gmode=VGAHI;
   void change();
   unsigned c[16]={ 0x0004,0x3ffe,0x00c0,0x00c0,0x08c0,0x08c8,\
		    0x0cfc,0x0cc0,0x0cc0,0x0cc0,0x0cc0,0x0cc2,\
		    0x7fff,0x0000};
   initgraph(&gdriver,&gmode,PATH);
   change(c);
   setcolor(2);
   for(i=0;i<16;i++)
     { setlinestyle(4,c[i],3);
       line(1,i+190,545,i+190);
       }
     getch();
     closegraph();
     }
   void change(unsigned *p)
    { unsigned a,b,i,j;
      for(i=0;i<16;i++)
       { a=0;
	 b=1;
	  for(j=0;j<16;j++)
	   { a<<=1;
	     if((*(p+i)&b)==1)
	     a|=b;
	     *(p+i)>>=1;
	     }
	   *(p+i)=a;
	   }
	  }