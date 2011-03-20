 #include "stdio.h"                         /*应用函数printf和scanf*/
 #define N 10
 static unsigned long k=-1;                 /*k为静态无符号整型全局变量*/
 void unsigned startnum(unsigned int seed)
  {
    k=seed;                                 /*返回k值*/
  }

 unsigned randnum(long n)                   /*n为形参*/
   {
     k=((k*159+23)%n)+1;
     return k;
   }
 int main()
 {
   register unsigned int i;                 /*寄存器变量*/
   long n=0;                                /*自动整型局部变量*/
   while(!(n>0&&(k>0&&k<=65536)))
     {
       printf("\n Input the seed and the max random[1-65535]:\n");
       scanf("%ld%ld",&k,&n);
       if(n<=0)
	 printf("\n wrong seed!");          /*显示错误信息*/
       if(k<=0||k>65536)
	 printf("\n wrong max random!");
     }
   startnum(n);                             /*n为实参,传值调用*/
   for(i=0;i<N;i++)
     printf("%6u",randnum(n));              /*循环求值并显示*/
   return;
 }


