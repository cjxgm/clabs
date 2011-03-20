main()
 {
   int i,j,num[4][6];
   printf("Enter 24 integers:\n");
   for(i=0;i<4;i++)
     for(j=0;j<6;j++)
       scanf("%d",&num[i][j]);
     for(i=0;i<4;i++)
      for(j=0;j<6;j++)
	if(num[i][j]<0)
	goto found ;
	printf("not found");
	goto done;
	found: printf("minus number num[%d][%d]:\%d\n",i,j,num[i][j]);
	done:;
	}