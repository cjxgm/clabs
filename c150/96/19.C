main()
 { int n;
   float e,p;
   e=p=1.0;
   for(n=1;n<=10;n++)
     {
       p=p*n;
       e=e+1.0/p;
       }
   printf("e=%10.7f\n",e);
   }