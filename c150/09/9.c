#define  N 17                       /*总人数*/
#define  M 3                        /*出局数字*/
#include "stdio.h"
main()
{
  int a[N],k=0,i,j;
  printf("\n\n The original orders are :\n");
  for(i=0;i<N;i++)                  /*初始位号并显示*/
   {
     a[i]=i+1;
     printf("%3d",a[i]);
   }
  printf("\n");
  for(i=0;k<N*M;i++)                /*循环实现出局*/
    {
      if(a[i%N]!=-1)
	{
	  k++;                       /*报数计数器*/
	  if(k%3==0)
	    {
	      a[i%N]=-1;             /*3倍数出局*/
	      printf("\n%2d is out!",(i%N+1));
	      if(k!=N*3)
		printf("The new order is:");
	      else
		printf("The game is over!");
	      for(j=0;j<N;j++)
		{
		  if(a[j]!=-1)
		   printf("%3d",a[j]);  /*出局信息显示*/
		 }
	     }
	 }
     }
  return;
}