 #include "stdio.h"
 #define  N 15                            /*定义最大组数*/   
 main()
  {
    int i,j,n,a[N][N];
    do{
       printf("\n Input n[1-12]:");
       scanf("%d",&n);
      }while(n<1||n>12);                   /*在范围内则开始计算*/
    for(i=0;i<=n;i++)
      {
	a[i][0]=1;                         /*外围赋值*/
	a[i][i]=1;
       }
     for(i=2;i<=n;i++)
       for(j=1;j<i;j++)
	  a[i][j]=a[i-1][j-1]+a[i-1][j];    /*对其它系数赋值*/
     printf("\n\n");
     for(i=0;i<=n;i++)
       {
	 for(j=0;j<(70-6*i)/2;j++)
	     printf(" ");                    /*显示前导空格*/
	 for(j=0;j<=i;j++)
	      printf("%6d",a[i][j]);          /*显示系数数字*/
	 printf("\n");
       }
     printf("\n");                            /*每组换行*/
     return;
 }