 #include "stdio.h"
  main()
   {
   int i,sum,a;
   a=2;
   do
    {
     i=1;
     sum=0;
      do
	{
	  if(a%i==0)
	  sum=sum+i;
	  i++;
	  }
       while(i<=a/2);
       if(sum==a)
       printf("%4d",a);
       a++;
     }
    while(a<500);
   }