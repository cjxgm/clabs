 #include "stdio.h"
 #define  N  50                           /*最大字符串数*/
 main()
  {
    int  up=0,                            /*大写字母*/
         low=0,                           /*小写字母*/
         digit=0,                         /*数字*/
         space=0,                         /*空格*/
	 other=0;                         /*其它字符*/
     char *p,s[N];
     printf("\n the string:\n");
     p=s;                                 /*指针指向字符串*/
     gets(p);                             /*得到字符串*/
     while(*p!='\0')                      /*循环移位判断*/
        {
          if((*p>='A')&&(*p<='Z'))
             up++;
	  else if((*p>='a')&&(*p<='z'))
             low++;
          else if(*p==' ')
             space++;
          else if((*p>='0')&&(*p<='9'))
              digit++;
          else
              other++;                    /*计数器自加*/
          p++;                            /*判断下一个字符*/
        }
      printf("\n up    letter:%d\n low   letter:%d\n space letter:%d\n digit letter:%d\n other letter:%d\n",up,low,space,digit,other);
      return;
   }