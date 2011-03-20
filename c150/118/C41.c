main()
 {
   int a,b,c,m;
   printf("\ninput three intergers:");
   scanf("%d %d %d",&a,&b,&c);
   m=min3(a,b,c);
   printf("min is %d",m);
   }
   int min2(int x, int y)
   { int z;
     z=x<y?x:y;
     return z;
     }
     int min3 (int x,int y,int z)
     { int w;
       w=min2(x,y);
       w=min2(w,z);
       return(w);
       }