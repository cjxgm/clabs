 #include "stdio.h"                     /*应用printf函数*/
 #include "stdlib.h"                    /*应用toupper函数*/
 void print()                           /*定义子函数*/
   {
      printf("\n\t This is an example.\n\n\t You are welcome!!\n\n");
   }
                          /* \n 表示换行; \t 表示移动8个字符位*/
void main()
  {
     int flag=1;                        /*定义整型变量并赋值*/
     char ch;                           /*定义字符变量*/
     while(flag)                        /*循环操作*/
       {
	 printf("\n Input 'F' or 'f' to start the program print,\n other to exit:");
	 ch=toupper(getchar());         /*得到大写字符*/
	 getchar();                     /*去掉回车符*/
	 if(ch=='F')
	   print();                     /*执行子函数*/
	 else
	   flag=0;
       }
     return;                            /*返回*/
  }

