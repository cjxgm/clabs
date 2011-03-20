main()
{ int m,n;
long amn;
long fac();
printf("input m,n;");
scanf("%d%d",&m,&n);
amn=fac(m);
amn=amn/fac(m-n);
printf("%ld\n",amn);}
long fac(int j)
 { long k;
   for(k=1;j>0;j--)
   k=k*j;
   return(k);
   }