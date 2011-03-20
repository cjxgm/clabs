#include "stdio.h"
void selectsort(int a[],int n)
{
  int i,j,k,num;
  for(i=0;i<n-1;i++)              /*外部循环实现选择排序*/
    {
      k=i;
      for(j=i+1;j<n;j++)          /*内部循环得到最小数*/
	if(a[j]<a[k])
	  k=j;
      if(k!=i)
	{
	  num=a[k];              /*交换排序最小数*/
	  a[k]=a[i];
	  a[i]=num;
	}
     }
  printf("\n\n The sorted numbers are:");
  for(i=0;i<n;i++)
    printf("%7d",a[i]);           /*显示排序的数字*/
}

void halfind(int a[],int n)       /*a为形参,地址传送*/
{
  int i,k,
  find=0,                         /*找到标志*/
  first=0,                        /*范围起点*/
  last=(n-1),                     /*范围终点*/
  half;                           /*查找点*/
  printf("\n\n Input the number to look for:");
  scanf("%d",&k);
  do{
      half=(first+last)/2;
      if(k==a[half])                /*根据判断结果执行下一步*/
	{
	  printf("\n Find %d,it is a[%d].",k,half);
	  find=1;
	}
      else if (k>a[half])
	 first=half+1;
      else
	 last=half-1;
     }while((first<=last)&&(find==0));   /*循环查找*/
   if(find==0)
     printf("\n %d not been found!",k);
}

main()
{
  int i,n,a[100];
  printf("\n Input the total number[1-100]:");
  scanf("%d",&n);
  printf("\n Input %d numbers:",n);
  for(i=0;i<n;i++)
    scanf("%d",&a[i]);
  printf("\n The original orders are:");
  for(i=0;i<n;i++)
     printf("%7d",a[i]);
  selectsort(a,n);
  halfind(a,n);                     /*a为实参,地址传送*/
  return;
}
