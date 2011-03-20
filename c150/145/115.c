# define N 5
void transp(int a[][N],int n)
 { int i,j,k;
 for(i=0;i<n;i++)
  for(j=0;j<i;j++)
   { k=a[i][j];
   a[i][j]=a[j][i];
   a[j][i]=k;
   }  }
  main()
  {  int n,i,j,x[N][N];
    printf("input n:");
    scanf("%d",&n);
    for(i=0;i<n;i++)
      for(j=0;j<n;j++)
	scanf("%d",&x[i][j]);
	transp(x,n);
	for(i=0;i<n;i++)
	 { for(j=0;j<n;j++)
	   printf("	%d",x[i][j]);
	   printf("\n");
	   }  }