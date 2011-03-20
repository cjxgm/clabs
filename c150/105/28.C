 # include "stdio.h"
  main()
   {
     long int x,y;
     int i,a[5];
     scanf("%ld",&x);
     if((x<10000)||(x>99999))
       {
	printf("This data is error");
	exit(0);
	}
       for(i=0;i<5;++i)
	{
	  a[i]=x%10;
	  x=x/10;
	  }
	fun(a);
	y=0;
	for(i=0;i<5;++i)
	 y=y*10+a[i];
	 printf("%ld",y);
	 }
       fun(int a[])
	{
	int i,j,k;
	for(i=0;i<5;++i)
	  for(j=i;j<5;++j)
	   if(a[j]<k)
	     { k=a[i];
	       a[i]=a[j];
	       a[j]=k;
	       }
	   }