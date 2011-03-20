main( )
  {   int  m, n, a, b, t, c;
      printf(" Input  two  integer  numbers :\n");   
      scanf(" %d %d",&a ,&b);
      m=a;
      n=b;                                  
      if(a<b)
       {   t=a ;
           a=b;
           b=t ;  }                     
      c= a%b ;
      while (c!=0)
      {  a=b ;
         b=c;
         c=a%b; }                  
      printf("The largest common divisor : %d\n",b);
      printf(" The largest common multiple : %d\n", m*n/b);
}
