main()
{ int i,j,k;
  clrscr();
  for(i=1;i<=9;i++)
   { for(j=1;j<30-3*i;j++)
     printf(" ");
     for(k=1;k<=i;k++)
       printf("  %d",k);
       for(k=i-1;k>0;k--)
	printf("  %d",k);
	printf("\n");
	}
	}