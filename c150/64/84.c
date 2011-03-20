# include "dos.h"
# include "conio.h"
main()
 { int i,j;
   union REGS inreg,outreg;
   inreg.h.ah=0;
   inreg.h.al=0x13;
   int86  (0x10,&inreg,&outreg);
   for(i=0;i<256;i++)
     for(j=0;j<200;j++)
      { inreg.h.ah=0x0c;
	inreg.h.al=i;
	inreg.h.ch=0;
	inreg.h.cl=i;
	inreg.h.dh=0;
	inreg.h.dl=j;
	int86 (0x10,&inreg,&outreg);
	}
	getch();
	inreg.h.ah=0;
	inreg.h.al=2;
	int86 (0x10,&inreg,&outreg);
	}