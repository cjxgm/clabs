 #include "stdio.h"
 #include "stdlib.h"                               /*应用malloc函数*/
 #include "ctype.h"                                /*应用toupper函数*/
 struct student                                    /*定义结构体*/
           {
              long number;                         /*学号*/
	      char name[20];                       /*姓名*/
	      float score;                         /*成绩*/
           };
 static int n=0;
 void new()                                        /*写新记录*/
  {
    FILE *fp;
    struct student stud;
    int flag=1;
    long k=n*sizeof(struct student);
    char ch,numstr[20];
    if((fp=fopen("stud.rec","wb"))==NULL)
      {
	printf(" can not open the file stud.rec");
	flag=0;
      }
    if(flag)
      {
	if(fseek(fp,k,0)!=0)
	  {
	    printf(" can not move pointer there.");
	    flag=0;
	  }
	if(flag)
	  {
	    printf("\n Input number:");
	    gets(numstr);
	    stud.number=atol(numstr);
	    printf("\n Input name:");
	    gets(stud.name);
	    printf("\n Input score:");
	    gets(numstr);
	    stud.score=atof(numstr);
	    fwrite(&stud,sizeof(struct student),1,fp);
	    n++;
	  }
      }
    fclose(fp);
  }
 void list()                                     /*显示记录*/
   {
     FILE *fp;
     struct student stud;
     int flag=1;
     long k;
     int  num=0;
     if(n==0)
      printf("\n It is empty! \n");
     else
       {
	 if((fp=fopen("stud.rec","rb"))==NULL)
	   {
	     printf(" can not open the file stud.rec");
	     flag=0;
	   }
	 if(flag)
	   {
	     while(!(num>=1&&num<=n))
	       {
		 printf("\n Input record number that you want[1-%d]:",n);
		 scanf("%d",&num);
		 getchar();
		 printf("\n record %d",num);
	       }
	     k=(num-1)*sizeof(struct student);
	     if(fseek(fp,k,0)!=0)
	       {
		 printf("\n Input can not move pointer there!");
		 flag=0;
	       }
	     if(flag)
	       {
		 fread(&stud,sizeof(struct student),1,fp);
		 printf("\n number:%ld",stud.number);
		 printf("\n name  :%s", stud.name);
		 printf("\n score :%6.2f",stud.score);
	       }
	    }
          fclose(fp);
        }
     }
 main()
   {
     char ch;
     int  flag=1;
     while(flag)                                   /*通用性设计*/
        {
	  printf("\n\n Input 'N' or 'n' to write a record,'L' or 'l' to list a record,\n other to exit:");
          ch=toupper(getchar());
          getchar();
          if(ch=='N')
            new();                                 /*写新记录*/
          else if(ch=='L')
	   list();                                 /*显示记录*/
          else
            flag=0;
         }
      return;
    }