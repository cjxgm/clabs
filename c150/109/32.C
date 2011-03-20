 # define nmax 40
  main()
   {
     int i,k,m,n,num[nmax],*p;
     printf("\nInput number :\n");
     scanf("%d",&n);
     p=num;
     for(i=0;i<n;i++)
      *(p+i)=i+1;
      i=0;
      k=0;
      m=0;
      while(m<n-1)
       {
       if(*(p+i)!=0)
       k++;
       if(k==3)
	{ *(p+i)=0;
	k=0;
	m++; }
	i++;
	if(i==n)
	i=0; }
	while(*p==0)
	p++;
	printf("%d is number %d.\n",n,*p);
	}
