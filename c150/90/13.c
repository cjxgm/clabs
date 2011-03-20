# include  "stdio.h"
   main( )
    {
      int x, n ;
      scanf("%d,%d",&x,&n);        
      printf("%d**%d=%d\n",x,n,power(x,n));      
     }
  power( int x, int n)
     {
       int  p;
        if (n>0)
          p=power(x, n-1)*x;
        else
          p=1;
       return(p);            
    }
