 #include "stdio.h"
 void binary(unsigned int number)             /*进制转换显示*/
    {
      int i,bit;
      unsigned int k=0x8000;
      printf(" %04x(16) \t",number);          /*显示16进制*/ 
      for(i=0;i<16;i++)
        {
          bit=(k&number)?1:0;
	  printf("%d",bit);                   /*显示2进制相应位数字*/
          if(i==7)
            printf("--");
           k>>=1;                             /*屏蔽数字*/
         }
       printf("(2)\n");
     }
 unsigned moveright(unsigned int number,int n)/*循环右移*/
    {
      unsigned int a,b;
      b=number>>n;
      a=number<<(16-n);
      number=a|b;                             /*组合数字*/
      return (number);
    }
 unsigned moveleft(unsigned int number,int n) /*循环左移*/
    {
      unsigned int a,b;
      b=number<<n;
      a=number>>(16-n);
      number=a|b;                             /*组合数字*/
      return (number);
    }
 main()
   {
     unsigned int number;
     int n=0;
     printf("\n Input a origenal number to move:");
     scanf("%d",&number);
     do{
         printf("\n Input number to move([-15,-1]or[1,15]):");
         scanf("%d",&n);
        }while(n<-15||n==0||n>15);
     printf("\n The origenal number is: %6d(10) \t");
     binary(number);
     if(n>0)                                  /*右移*/
       {
	 number=moveright(number,n);
	 printf("\n The result is:  \t %6d(10) \t",number);
	 binary(number);
       }
     else                                     /*左移*/
       {
	 n=-n;
	 number=moveleft(number,n);
	 printf("\n The result is:  \t %6d(10) \t",number);
         binary(number);
       }
     return;
   }