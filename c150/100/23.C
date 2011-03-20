 # include "stdio.h"
  main()
   {
   int a,k;
   printf("please input one number(0--1000)");
   scanf("%d",&a);
   if((a>0)&&(a<1000))
     {
     printf("%d",a);
     printf(" is groud of ");
     for(k=1;k<=a;++k)
     if(a%k==0)
     printf(" %d,",k);

     }
     else
     printf("This is a number out of group");
   }