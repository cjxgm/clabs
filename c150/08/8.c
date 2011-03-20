#include "stdio.h"
#define  M  200                   /*范围*/
#define  N  ((M+1)/2)             /*奇数的个数*/
main()
{
  int i,j,a[N];
  a[0]=2;
  for(i=1;i<N;i++)                /*初始数组实现第一步筛选*/
    a[i]=2*i+1;
  for(i=1;i<(N-1);i++)            /*第二步筛选*/
    if(a[i]!=0)
      for(j=i+1;j<N;j++)
	if((a[j]%a[i]==0))        /*倍数时置零筛去*/
	   a[j]=0;
  printf("\n The primer are[2-%d]:\n",M);
  for(i=0,j=0;i<N;i++)            /*显示得到素数*/
     {
       if(a[i]!=0)
	 {
	   printf("%5d",a[i]);
	   if((++j)%10==0)        /*十个数换行*/
	     printf("\n");
	 }
      }
   return;
 }
