 #include "stdio.h"
 #define  N  5
 #define  M  20
 main()
   {
     struct                           /*定义结构体*/
       {
	 char name[M];                /*姓名*/
	 int age;                     /*年龄*/
	 char sex;                    /*性别*/
	 char job;                    /*职业*/
	 union                        /*定义共用体*/
	    {
	      int class;              /*班级*/ 
	      char office[M];         /*办公室*/ 
	    }depart;                  /*单位*/
	}info[N];
      int i;
      for(i=0;i<N;i++)
	{
	  printf("\n Input name:");
	  gets(info[i].name);
	  printf("\n Input age:");
	  scanf("%d",&info[i].age);
	  getchar();
	  printf("\n Input sex(m/w):");
	  info[i].sex=getchar();
	  getchar();
	  printf("\n Input job(s/t):");
	  info[i].job=getchar();
	  getchar();
	  if(info[i].job=='s')          /*根据职业不同选择单位类型*/
	     {
	       printf("\n Input class:");
	       scanf("%d",&info[i].depart.class);
	       getchar();
	     }
	   else if(info[i].job=='t')
	      {
		printf("\n Input office:");
		gets(info[i].depart.office);
	      }
	   else
	      {
		printf("\n Input wrong job,return!");
		i--;
	      }
	 }
     printf("\n name \t\t age \t sex \t job \t depart ");
     for(i=0;i<N;i++)
       {
	 if(info[i].job=='s')          /*根据职业不同选择单位类型*/
	    printf("\n %-15s %-3d \t  %c \t  %-6c %-20d",info[i].name,info[i].age,info[i].sex,info[i].job,info[i].depart.class);
	 else
	    printf("\n %-15s %-3d \t  %c \t  %-6c %-20s",info[i].name,info[i].age,info[i].sex,info[i].job,info[i].depart.office);
	}
     return;
   }