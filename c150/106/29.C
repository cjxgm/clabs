 # include "stdio.h"
 main()
  {
   float a,b;
   int i;
   printf("input one number:");
   scanf("%f",&a);
   if(a>=4000)
    i=4;
    else
    i=a/1000;
    switch (i)
     { case 0:b=a*0.0; break;
       case 1:b=a*0.05; break;
       case 2:b=a*0.10; break;
       case 3:b=a*0.15; break;
       case 4:b=a*0.20; break;
       }
       printf("%f",b);
       }