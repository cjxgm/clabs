 # include "stdio.h"
  main()
  {
   int x,y,z,i,a,b;
    for(x=1;x<30;x++)
      for(y=1;y<=5;y++)
    {
       a=0;
       b=1;
       z=x;
       for(i=1;i<=5;i++)
	{
	a=a+z;
	b=b*z;
	z=z+y;
       }
       if(a==25&&b==945)
	for(i=0;i<12;i++)
	 printf("%4d",x+i*y);
    }
  }