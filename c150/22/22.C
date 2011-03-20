 #include "stdio.h"
 #include "stdlib.h"                  /*应用atol、atof函数*/
 #include "ctype.h"                   /*应用toupper函数*/
 #define  N  50                       /*最大记录数*/
 static int n=0;
 struct studinfo                      /*定义结构体*/
    {
      char name[10];                  /*姓名*/
      long number;                    /*学号*/
      char sex;                       /*性别*/
      float score;                    /*成绩*/
    }student[N];
 void new()                           /*输入信息*/
    {
      char numstr[10];
      printf("\n Input all data(name,number,sex(m/w),score)of studene[%d]:\n",n+1);
      gets(student[n].name);
      gets(numstr);
      student[n].number=atol(numstr);
      student[n].sex=getchar();
      getchar();
      gets(numstr);
      student[n].score=atof(numstr);
      n++;                             /*记录个数*/
    }
 void listone(int k,struct studinfo *s)/*输出单个记录*/
    {
      printf(" %6d \t %-12s \t %-8ld \t  %c \t %6.2f \n",k,s->name,s->number,s->sex,s->score);
    }
 void list()                           /*输出记录*/
    {
      int i;
      if(n<1)
          printf("\n There is not a student!");
      else
        {
	  printf("\n record \t name \t \t number \t sex \t score\n");
          for(i=0;i<n;i++)
             listone(i+1,&student[i]);
         }
     }
 main()
   {
     char ch;
     int  flag=1;
     while(flag==1)                      /*通用性设计*/
        {
          printf("\n Input 'N' or 'n' to input new record,'L'' or 'l' to list all record:");
          ch=toupper(getchar());
          getchar();
          if(ch=='N')
            new();
          else if(ch=='L')
            list();
          else
            flag=0;
         }
      return;
    }     
   