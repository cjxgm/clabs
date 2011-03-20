 #define N 20
  main()
   { int i,j;
      float a[N],temp,*p;
      p=a;
      printf("\n Input %d numbers\n",N);
      for(i=0;i<N;i++)
	scanf("%f",p+i);
	for(i=0;i<N-1;i++)
	 for(j=i+1;j<N;j++)
	   if( *(p+i)< *(p+j) )
	     {  temp=*(p+i);
		*(p+i)=*(p+j);
		*(p+j)=temp;
		}
	   for(i=0;i<N;i++)
	     { if(i%5==0)
	       printf("\n");
	       printf("%8.3f",*(p+i));
	       }
	   }