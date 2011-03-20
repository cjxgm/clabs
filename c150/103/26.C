 # include "stdio.h"
  main()
   {
   int i , j;
    i=1;
    j=0;
   for( i=1;i<=1000;i++)
    {
      if(i%3==0&&i%5==0&&i%7==0)
       {
	printf("%4d",i);
	j=j+1;
	}
       if(j==4)
	{
	 printf("\n");
	 j=0;
	 }
       }
    }