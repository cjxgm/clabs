main()
 {
   int i,j,num[3][4];
   printf("Enter 12 integers:\n");
   for(i=0;i<3;i++)
     for(j=0;j<4;j++)
       scanf("%d",&num[i][j]);
     for(i=0;i<3;i++)
      for(j=0;j<4;j++)
	if(num[i][j]<0)
	goto found ;
	printf("not found");
	goto done;
	found: printf("minus number num[%d][%d]:\%d\n",i,j,num[i][j]);
	done:;
	}