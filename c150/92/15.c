# include "stdio.h"
 main()
  {
    int le,nu,ot,i;
    char c, t[100];
    i=0;le=0;nu=0;ot=0;
    while((c=getchar())!='\n')
      {
	t[i]=c;
	if(('a'<=t[i]&&t[i]<='z')||('A'<=t[i]&&t[i]<='Z'))
	   ++le;
       else if ('0'<=t[i]&&t[i]<='9')
	  ++nu;
	  else
	  ++ot;
	  i++;
	  }
	  printf("%d le, %d nu,%d ot\n",le,nu,ot);
	  }