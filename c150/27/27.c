 #include "stdio.h"
 main()
   {
     enum colours                          /*定义枚举类型*/
	{
	  RED,YELLOW,BLUE,GREEN,WHITE,BLACK
	};
     enum colours i,j,k,p;                 /*定义枚举变量*/
     int n=0,number;
     for(i=RED;i<=BLACK;i++)               /*穷举比较*/
	for(j=RED;j<=BLACK;j++)
	  {
	    if(i!=j)
	      {
		for(k=RED;k<=BLACK;k++)
		   if(k!=i&&k!=j)
		      {
			if(n%3==0)         /*每行显示3种情况*/
			   printf("\n");
			printf("%-4d",++n);/*新排列组合则输出结果*/
			for(number=1;number<=3;number++)
			   {
			     switch (number)/*取枚举常量值*/
			       {
				 case 1 : p=i;break;
				 case 2 : p=j;break;
				 case 3 : p=k;break;
				 defaut :     break;
				}
			      switch (p)
				{
				  case RED    : printf("%-7s","red");break;
				  case YELLOW : printf("%-7s","yellow");break;
				  case BLUE   : printf("%-7s","blue");break;
				  case GREEN  : printf("%-7s","green");break;
				  case WHITE  : printf("%-7s","white");break;
				  case BLACK  : printf("%-7s","black");break;
				}
			   }
		       }
		 }
	    }
     printf("\n\n total: %4d \n",n);
     return;
   }