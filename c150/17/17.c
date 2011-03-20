 #include "stdio.h"
 int atoi(char *p)               /*转换子函数*/
   {
     int k,number=0;
     while(*p!='\0'&&(*p>='0')&&(*p<='9'))             /*未到最后则循环读取每一位数字*/
        {
          k=*(p++)-48;
          number=10*number+k;    /*得到每一位数字*/
         }
     return number;              /*返回整型数值*/
    }
 main()
   {
     char s[10];
     int number;
	printf("\n\n Input a digit string:");
	gets(s);                     /*输入整型字符串*/
     number=atoi(s);              /*传递指针*/
     printf("\n\n %d",number);
     return;
    }