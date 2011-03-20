 #include "string.h"
 #include "stdio.h"
 #define N 50
 main()
  {
    int i,j,k=0;
    char str[2][N],                 /*存放原始字符串*/
         flag[N][5];                /*存放比较结果*/ 
    printf("\n\n Input the first string shorter than %d letters:",N);
    gets(str[0]);
    printf("\n\n Input the second string shorter than %d letters:",N);
    gets(str[1]);                   /*得到两字符串*/  
    for(i=0;i<N;i++)
      for(j=0;j<5;j++)
	flag[i][j]='-';             /*标志信息数组置初值*/
    for(i=0;i<N;i++)                /*对每一个字符进行比较*/
      {

	if(str[0][i]=='\0')          /*字符串1长度小于字符串2时*/
	  for(j=i;(j<N)&&(str[1][j]!='\0');j++)
	    {
	      flag[j][0]='y';
	      flag[j][4]=str[1][j];
	    }
	else  if(str[1][i]=='\0')     /*字符串2长度小于字符串1时*/
	  for(j=i;(j<N)&&(str[0][j]!='\0');j++)
	    {
	      flag[j][0]='y';
	      flag[j][2]=str[1][j];
	    }
	 else if(str[0][i]!=str[1][i]) /*字符不同时*/
	    {
	      flag[i][0]='y';
	      flag[i][2]=str[0][i];
	      flag[i][4]=str[1][i];
	    }
       }
    for(i=0;i<N;i++)                    /*显示比较结果*/
      {
	if(flag[i][0]=='y')
	  {
	    if((k++%8)==0)
	      printf("\n");
	    printf("%4d",(i+1));
	    for(j=1;j<5;j++)
	       printf("%c",flag[i][j]);
	  }
      }
   return;
 }


