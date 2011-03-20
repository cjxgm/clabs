 # define N1 3
 # define N2 4
 main()
  { int a[N1][N2],max,min,(*ap)[N2];
    int i,j,row1,row2,column1,column2;
    ap=a;
    row1=row2=column1=column2=0;
    for(i=0;i<N1;i++)
      for(j=0;j<N2;j++)
      scanf("%d",*(ap+i)+j);
      max=min=a[0][0];
      for(i=0;i<N1;i++)
	for(j=0;j<N2;j++)
	  { if ( max<*(*(ap+i)+j) )
	    { max=*(*(ap+i)+j);
	      row1=i;
	      column1=j;  }
	    if( min> *(*(ap+i)+j))
	       { min=*(*(ap+i)+j);
	       row2=i;
	       column2=j; }
	  }
      printf("\nmax=%d\trow1=%4d column1=%4d\n",max,row1+1,column1+1);
      printf("\nmin=%d\trow2=%4d column2=%4d\n",min,row2+1,column2+1);
      }
