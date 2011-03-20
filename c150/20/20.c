 #include "stdio.h"
 #include "string.h"               /*应用strcmp函数*/
 #define LENGTH  30                /*定义字符串最大长度*/
 #define N  10                     /*定义字符个数*/
 void sort(char **p)               /*冒泡排序*/
  {
    int i,j;
    char *pstr;
    for(i=0;i<N;i++)
      for(j=i+1;j<N;j++)
        if(strcmp(*(p+i),*(p+j))>0) /*比较后交换字符串地址*/
          {
            pstr=*(p+j);
            *(p+j)=*(p+i);
            *(p+i)=pstr;
           }
   }

 main()
  {
    int i;
    char *pstr[N],s[N][LENGTH];
    for(i=0;i<N;i++)
      pstr[i]=s[i];         /*将第i个字符串的首地址赋予指针数组*/
   printf("\n Input %d strings:\n", N);
    for(i=0;i<N;i++)
      gets(pstr[i]);                  /*输入字符串*/
    sort(pstr);
    printf("\n The sorted string are:\n");
    for(i=0;i<N;i++)
      puts(pstr[i]);
    return;
  }

