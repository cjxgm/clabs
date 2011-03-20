 #include "stdio.h"
 main()
  {
    int a=0,                                 /*原始进制*/
	b=8,                                 /*转换进制，默认为八*/  
	k=1;                                 /*位权*/
    long xa=0,                               /*原始数值*/
	 xb=0,                               /*转换数值*/
	 x1=0,x2=0;                          /*十进制数值*/
    printf("\n Input the number and weight a->b:");
    scanf("%ld %d",&xa,&a,&b);
    x1=xa;
    while(x1!=0)                             /*转换为十进制数*/
      {
	x2+=(x1%10)*k;                       /*逐位取值*/
	x1/=10;
	k*=a;
      }
    k=1;
    x1=x2;
    while(x1!=0)                             /*转换为b进制数*/
      {
	xb+=(x1%b)*k;
	x1/=b;
	k*=10;
      }
    printf("\n %ld(%d)=%ld(%d)=%ld(%d)",xa,a,x2,10,xb,b);
    return;
 }