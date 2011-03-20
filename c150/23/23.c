 #include "stdio.h"
 #include "stdlib.h"                                /*应用malloc函数*/
 #define  LENGTH  sizeof(struct number)             /*内存空间*/
 struct number                                      /*定义结构体*/
           {
	      long key;                             /*数字*/
	      int count;                            /*计数器*/
	      struct number *next;                  /*连接指针*/
           };
 struct number *add(long k,struct number *head)
    {
      int flag=1;
      struct number *p=head;
      while(p!=NULL&&flag==1)
          {
            if(p->key==k)
              flag=0;
            else
               p=p->next;
           }
       if(flag==0)                                  /*存在已知数*/
            p->count++;
       else                                         /*在表头添加*/
          {
            p=head;
	    head=(struct number *)malloc(LENGTH);   /*开辟空间*/
            head->key=k;
            head->count=1;
            head->next=p;
           }
       return(head);
     }
 void list(struct number *p)                        /*输出记录*/
    {
      printf(" \n The result are:\n");
      while(p!=NULL)
	{
	  printf("%16ld %6d\n",p->key,p->count);
          p=p->next;
	}
    }
 main()
   {
     struct number *head=NULL;
     long k;
     printf("\n Input natural numbers,-1 to end:\n");
     scanf("%ld",&k);
     while(k>=0)
        {
          head=add(k,head);
          scanf("%ld",&k);
         }
     list(head);
     return;
   }