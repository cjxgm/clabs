# include  "stdlib.h"
main( )
  {
    int a ,b , c , x=0;
    while (x<5)
     {
       a=rand( )%100 ;
       b=rand( )%100 ;
       if (a+b >100)
        continue;
     printf("\n%d+%d=?  ",a,b) ;
      scanf ("%d",&c);
      if (c==a+b)
       { printf("The  answer  is  rignt ");
          x++;
         }
      else
         { printf( " The  answer  is wrong!");
           x=0;
         }
     }
  printf("\n Program  is  over.");       
}
