# include <stdlib.h>
# include "vga13h.h"
# include <dos.h>

void main()
{
  int i,j;
  unsigned int x,y,c;
  setmode();
  for (x=1;x<=32;x++)
  {
    setpal(x,    x*2-1, 0,     0    );
    setpal(x+32, 63,    x*2-1, 0    );
    setpal(x+64, 63,    63,    x*2-1);
    setpal(x+96, 63,    63,    63   );
  }
  while(ScanKey()!=1)
  {
   /*
   for (i=0;i<16;i++)
       for (j=0;j<16;j++)
	   pset(random(300)+j,random(150)+i,random(256));
   */
   for (x=0;x<320;x+=2)
   {
     for (y=1;y<=202;y+=2)
     {
       c=(point(x,y)+point(x+2,y)+point(x,y+2)+point(x+2,y+2))>>2;
       if (c--)
       {
	  poke(0xa000,(y-2)*320+x,(c<<8)+c);
	  poke(0xa000,(y-1)*320+x,(c<<8)+c);
       }
     }
     y-=2;
     pset(x,y,random(320));
   }
  }
  closemode();
}
