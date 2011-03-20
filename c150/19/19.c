 #include "stdio.h"
 #define  N  4                           /*科数*/
 #define  M  5                           /*人数*/
 void search(int (*p)[N])              /*指针指向含有n个元素的数组*/
    {
      int i,j,flag;
      for(i=0;i<M;i++)
        {
          flag=0;
          for(j=0;j<N;j++)
	    if(*(*(p+i)+j)<60)           /*比较相应成绩:第i+1人第j+1门*/
               flag=1;                  /*小于60做标记*/
           if(flag==1)                  /*符合条件则显示*/
             {
               printf("\n\n N0.%d is flunked,score are:",(i+1));
               for(j=0;j<N;j++)
		  printf("%5.1d",(*(*(p+i)+j)));

             }
         }
      }

 main()
  {
    int i,j;
    int score[M][N];
    for(i=0;i<M;i++)                    /* 得到初始成绩*/
      {
	printf("\n Input %d scores of N0.%d :\n",N,i+1);
        for(j=0;j<N;j++)
	   scanf("%d",&score[i][j]);
      }
    search(score);
    return;
  }