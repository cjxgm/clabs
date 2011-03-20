# include "dos.h"
# include "conio.h"
main()
 {  int i,j;
    _AX=0X13;
    geninterrupt(0x10);
    for(i=0;i<64;i++)
     {  _AX=0x1010;
       _BX=i;
       _CH=i;
       _CL=0;
       _DH=0;
       geninterrupt(0x10);
       _AX=0x1010;
       _BX=i+64;
       _CH=0;
       _CL=i;
       _DH=0;
       geninterrupt(0x10);
       _AX=0x1010;
       _BX=i+128;
       _CH=0;
       _CL=0;
       _DH=i;
       geninterrupt(0x10);
       }
       for(i=0;i<192;i++)
	 for(j=0;j<200;j++)
	   { _AH=0x0c;
	    _AL=i;
	    _CX=j;
	    _DX=i;
	    geninterrupt(0x10);
	    }
	    getch();
	    _AX=0x0002;
	    geninterrupt(0x10);
	    }

