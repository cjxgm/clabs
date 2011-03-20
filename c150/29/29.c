 #include "stdio.h"
 #include "ctype.h"
 #include "string.h"
 void found()                                   /*建立新文件*/                         
   {
     FILE *fp;
     char *p;
     int flag=1;
     if((fp=fopen("file1.txt","w"))==NULL)       /*写打开*/
       {
	 printf("\n can not open file1.txt !");
	 flag=0;
	 fclose(fp);
       }
     while(flag)
       {
	 printf("\n Input letters:");
	 while(strlen(gets(p))>0)
	   {
	     fputs(p,fp);                         /*写字符串*/
	     fputs("\n",fp);
	     fclose(fp);
	     flag=0;
	     printf("\n Found file1.txt !");
	   }
       }
   }
 char new(char ch)                                /*加密规律*/
   {
     if((ch>=32)&(ch<122))
        if(ch==122)
          ch=32;
        else
          ch=ch+1;
     return ch;
   }
 void change()                                    /*文件加密*/
   {
     FILE *fp1,*fp2;
     char ch;
     int flag=1;
     if((fp1=fopen("file1.txt","r"))==NULL)       /*读打开*/
       {
	 printf("\n can not open file2.txt !\n");
	 flag=0;
	 fclose(fp1);
       }
     if(flag)
       if((fp2=fopen("file2.txt","w"))==NULL)
	 {
	   printf("\n can not open file2.txt !\n");
	   flag=0;
	   fclose(fp2);fclose(fp1);
	 }
     while(flag)
       {
	 while((ch=fgetc(fp1))!=EOF)                 /*读字符*/
	   {
	      ch=new(ch);
	      fputc(ch,fp2);                         /*写字符*/
	   }
	 flag=0;
	 fclose(fp1);fclose(fp2);
	 printf("\n source file: file1.txt \n new file: file2.txt");
       }
   }
 main()
   {
     char ch;
     int flag=1;
     while(flag)                                      /*通用性设计*/
       {
	 printf("\n Input 'F' or 'f' to found a new source file;\n 'C' or 'c' to change a file;other to exit:");
	 ch=toupper(getchar());getchar();
	 if(ch=='F')
	   found();
	 else if(ch=='C')
	   change();
	 else
	   flag=0;
       }
   return;
 }