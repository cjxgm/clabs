 # include "stdio.h"
  main()
   {
     void search( float (*p)[4],int m);
     static float score[5][4]={{65,80,78,90},{98,89,100,83},
		      {92,56,78,70},{78,63,80,70},{64,55,70,81}};
		      clrscr();
     search(score,5);
     }
     void search ( float (*p)[4],int m)
      {
       int i,j,flag;
       for(i=0;i<m;i++)
	{
	flag=0;
	for(j=0;j<4;j++)
	 { if(*(*(p+i)+j)<60)  flag=1;
	 if (flag==1)
	   {
	    printf("Np.%d is flunked,score are:\n",i+1);
	    for(j=0;j<4;j++)
	    printf("%5.1f",*(*(p+i)+j));
	    printf("\n");
	    }
	   }
	 }
       }