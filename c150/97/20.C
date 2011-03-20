 #include "stdio.h"
   main()
    { int k,n;
      n=0;
      k=2;
      while(n<15)
      {   if(isprime(k)&&isprime(k+2))
	  {
	    n=n+1;
	    printf("%d,%d",k,k+2);
	    }
	  k=k+1;}
	  }
      isprime (int a)
	{
	 int k,j;
	 j=1;
	 k=2;
	 while ((k<=a/2)&&j)
	  {
	    if(a%k==0)
	     j=0;
	     else
	     k=k+1;
	     }
	     return(j);}