 #include "stdio.h"
 #include "stdlib.h"                               /*应用malloc函数*/
 #include "ctype.h"                                /*应用toupper函数*/
 #include "string.h"                               /*应用strcpy函数*/
 #define  LENGTH  sizeof(struct student)           /*内存空间*/
 static int n=0;
 struct student                                    /*定义结构体*/
           {
              long number;                         /*学号*/
	      char name[20];                       /*姓名*/
	      struct student *next;                /*连接指针*/
           };
 struct student *new(struct student *head)
    {
      char numstr[20];
      struct student *p,*p1=head,*p2=head;
      p=(struct student *)malloc(LENGTH);          /*开辟空间*/
      printf("\n Input the new record`s number:"); /*输入数据*/
      gets(numstr);
      p->number=atol(numstr);
      printf("\n Input the new record`s name:");
      gets(p->name);
      if(p->number>0)                              /*确定位置*/
         {
           if(head==NULL)                          /*插入空表*/
              {
                head=p;
		p->next=NULL;
		n++;
              }
	   else
	     {
	       while((p->number>p1->number)&&(p1->next!=NULL))
                 {
                   p2=p1;                          /*确定位置*/
                   p1=p1->next;
                 }
	       if(p->number<p1->number)
		 {
		   if(head==p1)                     /*插入表头*/
		     {
		       head=p;
		       p->next=p1;
		       n++;
		     }
		   else                            /*插入表中*/
		     {
		       p2->next=p;
		       p->next=p1;
		       n++;
		     }
		 }
               else if(p->number==p1->number)      /*相等则覆盖*/
		  strcpy(p1->name,p->name);
	       else if(p1->next==NULL)             /*插入表尾*/
		 {
		   p1->next=p;
	           p->next=NULL;
	           n++;
	         }
	     }
	  }
      else
	 printf("\n Input wrong number! \n");       /*错误学号*/
      return(head);
    }
 void list(struct student *head)                    /*输出记录*/
    {
      struct student *p=head;
      int i=1;
      if(head!=NULL)
        {
	  printf("\n Now  %d records are:\n\n record \t number \t name \n",n);
             do{
		 printf("\n %6d \t %-16ld",i++,p->number);
                 puts(p->name);
                 p=p->next;
               }while(p!=NULL);
	}
      else
	 printf("\n\n It is empty! \n\n");
    }
 struct student *delete(struct student *head)       /*删除记录*/
   {
     struct student *p1,*p2;
     long k;
     char numstr[20];
     printf("\n Input the number that you want to delete:");
     k=atol(gets(numstr));
     if(head==NULL)                                 /*空表*/
       printf("\n this is an empty list!\n");
     else if(head->number==k)                       /*删表头*/
       {
	 p1=head;
	 head=head->next;
	 free(p1);                                  /*释放空间*/
         n--;
       }
     else                                           /*非表头*/
       {
	 p1=head;
	 while(p1->number!=k&&p1->next!=NULL)
	   {
	     p2=p1;                                 /*确定位置*/
	     p1=p1->next;
	   }
	 if(p1->number==k)                          /*存在则删除*/
	   {
	     p2->next=p1->next;
	     free(p1);                              /*释放空间*/
	     n--;
           }
	 else                                       /*显示错误信息*/
	   printf("\n The record is not in this list.\n");
	}
      return(head);
    }
 main()
   {
     struct student *head;
     char ch;
     int  flag=1;
     if(n==0)
        head=NULL;
     while(flag)                                   /*通用性设计*/
        {
	  printf("\n Input 'N' or 'n' to input new record,'L' or 'l' to list all record,\n 'D' or 'd' to delete a record,other to exit:");
          ch=toupper(getchar());
          getchar();
          if(ch=='N')
            head=new(head);                        /*写新数据*/
          else if(ch=='L')
            list(head);                            /*显示记录*/
          else if(ch=='D')                         /*删除数据*/ 
	    head=delete(head);
          else
            flag=0;
         }
      return;
    }