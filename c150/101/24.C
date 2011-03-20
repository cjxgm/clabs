 # define N 10
  main()
   {
   int a[12],p,x,*t;
   for(t=a+1;t<=a+N;t++)
     scanf("%d",t);
     scanf("%d",&x);
     t=a;
     p=1;
     while (x>*(t+p)&&p<=N)
     p++;
     for(t=a+N;t>=a+p;t--)
       *(t+1)=*t;
       t=a+p;
       *t=x;
       for(t=a+1;t<=a+N+1;t++)
       printf("%5d",*t);
     }